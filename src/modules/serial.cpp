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

/**
 * @brief Set the baudrate of the USART peripheral
 * @param USARTx pointer to USART peripheral
 * @param baudrate desired baudrate
 * @return en_result_t Ok if successful, ErrorInvalidParameter otherwise
 * 
 * @note copy of SetUartBaudrate in hc32f460_usart.c, but utilizing fixed point math instad of float
 */
en_result_t SetUartBaudrate_FP(M4_USART_TypeDef *USARTx, const uint32_t baudrate)
{
  // get base clock frequency
  const uint32_t C = SystemCoreClock / (1ul << (2ul * USARTx->PR_f.PSC));
  ASSERT(C > 0, "USART peripheral clock is zero");

  // get OVER8 oversampling setting
  const uint32_t OVER8 = USARTx->CR1_f.OVER8;

  // calculate integer part of divider
  const uint32_t div_int = (C / (baudrate * 8 * (2 - OVER8))) - 1;
  if (div_int > 0xFF)
  {
    return ErrorInvalidParameter;
  }

  // calculate fractional part of divider only if deviation is not negligible
  uint32_t div_fract = 0xFFFFFFFF;
  if ((C % (baudrate * 8 * (2 - OVER8))) != 0)
  {
    const uint64_t fractTmp = (static_cast<uint64_t>(2 - OVER8) * static_cast<uint64_t>(div_int + 1) * static_cast<uint64_t>(baudrate));
    div_fract = (2048 * fractTmp / C) - 128;

    if (div_fract > 0x7F)
    {
      return ErrorInvalidParameter;
    }
  }

  // set baudrate registers
  USARTx->CR1_f.FBME = div_fract > 0x7F ? 0ul : 1ul;
  USARTx->BRR_f.DIV_FRACTION = div_fract;
  USARTx->BRR_f.DIV_INTEGER = div_int;
  return Ok;
}


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

  // set tx pin
  PORT_SetFunc(tx_pin.port, tx_pin.pin, USART_DEV_TO_TX_FUNC(peripheral), Disable);

  // enable USART clock
  PWC_Fcg1PeriphClockCmd(USART_DEV_TO_PERIPH_CLOCK(peripheral), Enable);

  // initialize USART peripheral and set baudrate
  USART_UART_Init(peripheral, &usart_config);
  SetUartBaudrate_FP(peripheral, baudrate);
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

    // disable USART clock
    PWC_Fcg1PeriphClockCmd(USART_DEV_TO_PERIPH_CLOCK(peripheral), Disable);
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

#if HAS_SERIAL(HOST_SERIAL)
  Serial hostSerial(
    CONCAT(M4_USART, HOST_SERIAL), 
    HOST_SERIAL_TX
  );
#endif

#if HAS_SERIAL(SCREEN_SERIAL)
  Serial screenSerial(
    CONCAT(M4_USART, SCREEN_SERIAL), 
    SCREEN_SERIAL_TX
  );
#endif
