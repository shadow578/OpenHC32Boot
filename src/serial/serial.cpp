#include "serial.h"

void serial_init()
{
    // create common USART configuration
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

    // initialize host serial
    usart_sync_init(HOST_SERIAL_PERIPHERAL, HOST_SERIAL_TX_PIN, HOST_SERIAL_BAUDRATE, &usart_config);

#if ENABLE_UI == 1
    // initialize screen serial
    usart_sync_init(SCREEN_SERIAL_PERIPHERAL, SCREEN_SERIAL_TX_PIN, SCREEN_SERIAL_BAUDRATE, &usart_config);
#endif
}
