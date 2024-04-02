#include "serial.h"
#include <drivers/usart/usart_sync.h>

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

  usart_sync_init(peripheral, tx_pin, baudrate, &usart_config);
}

void Serial::put(const uint8_t ch)
{
  usart_sync_putc(peripheral, ch);
}

void Serial::write(const char *str)
{
  usart_sync_write(peripheral, str);
}
