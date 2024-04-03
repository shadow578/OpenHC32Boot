/**
 * serial driver based on the Arduino core's usart_sync driver
 */
#include "serial.h"
#include <addon_usart.h>

// USART_TypeDef to gpio function select mapping
#define USART_DEV_TO_TX_FUNC(usart)       \
    usart == M4_USART1   ? Func_Usart1_Tx \
    : usart == M4_USART2 ? Func_Usart2_Tx \
    : usart == M4_USART3 ? Func_Usart3_Tx \
                         : Func_Usart4_Tx

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
  USART_FuncCmd(peripheral, UsartRx, Disable);
  USART_FuncCmd(peripheral, UsartRxInt, Disable);
  USART_FuncCmd(peripheral, UsartTx, Disable);
  USART_FuncCmd(peripheral, UsartTxEmptyInt, Disable);
  USART_FuncCmd(peripheral, UsartTxCmpltInt, Disable);
  USART_DeInit(peripheral);

  // set tx pin function to USART TX output
  PORT_SetFunc(tx_pin.port, tx_pin.pin, USART_DEV_TO_TX_FUNC(peripheral), Disable);

  // enable USART clock
  PWC_Fcg1PeriphClockCmd(USART_DEV_TO_PERIPH_CLOCK(peripheral), Enable);

  // initialize USART peripheral and set baudrate
  USART_UART_Init(peripheral, &usart_config);
  SetUartBaudrate(peripheral, baudrate);
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

#if defined(HOST_SERIAL) && HOST_SERIAL != -1
  Serial hostSerial(CONCAT(M4_USART, HOST_SERIAL), HOST_SERIAL_TX);
#endif

#if defined(SCREEN_SERIAL) && SCREEN_SERIAL != -1
  Serial screenSerial(CONCAT(M4_USART, SCREEN_SERIAL), SCREEN_SERIAL_TX);
#endif
