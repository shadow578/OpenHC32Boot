/**
 * serial driver based on the Arduino core's usart_sync driver
 */
#include "serial.h"
#include "delay.h"
#include "assert.h"
#include <addon_usart.h>

// USART_TypeDef to gpio function select mapping
#define USART_DEV_TO_TX_FUNC(usart)       \
    usart == M4_USART1   ? Func_Usart1_Tx \
    : usart == M4_USART2 ? Func_Usart2_Tx \
    : usart == M4_USART3 ? Func_Usart3_Tx \
                         : Func_Usart4_Tx

#define USART_DEV_TO_RX_FUNC(usart)       \
    usart == M4_USART1   ? Func_Usart1_Rx \
    : usart == M4_USART2 ? Func_Usart2_Rx \
    : usart == M4_USART3 ? Func_Usart3_Rx \
                         : Func_Usart4_Rx

// USART_TypeDef to PWC_FCG1_PERIPH_USARTx mapping
#define USART_DEV_TO_PERIPH_CLOCK(usart)          \
    usart == M4_USART1   ? PWC_FCG1_PERIPH_USART1 \
    : usart == M4_USART2 ? PWC_FCG1_PERIPH_USART2 \
    : usart == M4_USART3 ? PWC_FCG1_PERIPH_USART3 \
                         : PWC_FCG1_PERIPH_USART4

void Serial::init(uint32_t baudrate)
{
  const stc_usart_uart_init_t usart_config = {
    .enClkMode = UsartIntClkCkNoOutput,    // no output on SCK pin
    .enClkDiv = UsartClkDiv_1,             // 1:1 clock divider
    .enDataLength = UsartDataBits8,        // 8 data bits
    .enDirection = UsartDataLsbFirst,      // LSB first
    .enStopBit = UsartOneStopBit,          // 1 stop bit
    .enParity = UsartParityNone,           // no parity
    .enSampleMode = UsartSampleBit8,       // 8 bit sampling
    .enDetectMode = UsartStartBitFallEdge, // start bit on falling edge
    .enHwFlow = UsartRtsEnable,            // RTS enabled
  };

  // disable and de-init usart peripheral
  deinit();

  // set rx and tx pins
  PORT_SetFunc(tx_pin.port, tx_pin.pin, USART_DEV_TO_TX_FUNC(peripheral), Disable);

  if (has_rx)
  {
    PORT_SetFunc(rx_pin.port, rx_pin.pin, USART_DEV_TO_RX_FUNC(peripheral), Disable);
  }

  // enable USART clock
  PWC_Fcg1PeriphClockCmd(USART_DEV_TO_PERIPH_CLOCK(peripheral), Enable);

  // initialize USART peripheral and set baudrate
  USART_UART_Init(peripheral, &usart_config);
  SetUartBaudrate(peripheral, baudrate);

  // enable RX function
  if (has_rx)
  {
    USART_FuncCmd(peripheral, UsartRx, Enable);
  }
}

void Serial::deinit()
{
  #if SKIP_USART_DEINIT != 1
    // implicitly done by USART_DeInit:
    //USART_FuncCmd(peripheral, UsartRx, Disable);
    //USART_FuncCmd(peripheral, UsartRxInt, Disable);
    //USART_FuncCmd(peripheral, UsartTx, Disable);
    //USART_FuncCmd(peripheral, UsartTxEmptyInt, Disable);
    //USART_FuncCmd(peripheral, UsartTxCmpltInt, Disable);

    USART_DeInit(peripheral);
  #endif
}

void Serial::put(const uint8_t ch)
{
  // enable TX function
  USART_FuncCmd(peripheral, UsartTx, Enable);

  // wait until TX buffer is empty
  while (USART_GetStatus(peripheral, UsartTxEmpty) == Reset) { /* nada */ }

  // write char to TX buffer
  USART_SendData(peripheral, ch);
}

void Serial::write(const char *str)
{
  while (*str != '\0')
  {
    put(*str++);
  }
}

size_t Serial::read(uint8_t *buffer, const size_t length, const uint16_t timeout)
{
  ASSERT(has_rx, "this Serial instance does not support reading");

  size_t read_bytes = 0;
  for (; read_bytes < length; read_bytes++)
  {
    // wait until RX buffer is not empty
    uint32_t time = 0;
    while (USART_GetStatus(peripheral, UsartRxNoEmpty) != Set)
    {
      delay::us(1);
      time++;
      if (time > (timeout * 1000))
      {
        // timeout reached, return however many bytes were read
        return read_bytes;
      }
    }

    // buffer full, read the byte
    buffer[read_bytes] = static_cast<uint8_t>(USART_RecData(peripheral));
    read_bytes++;
  }

  return read_bytes;
}

#if HAS_SERIAL(HOST_SERIAL)
  Serial hostSerial(
    CONCAT(M4_USART, HOST_SERIAL), 
    HOST_SERIAL_TX
    #if defined(HOST_SERIAL_RX)
      , HOST_SERIAL_RX
    #endif
  );
#endif

#if HAS_SERIAL(SCREEN_SERIAL)
  Serial screenSerial(
    CONCAT(M4_USART, SCREEN_SERIAL), 
    SCREEN_SERIAL_TX
    #if defined(SCREEN_SERIAL_RX)
      , SCREEN_SERIAL_RX
    #endif
  );
#endif
