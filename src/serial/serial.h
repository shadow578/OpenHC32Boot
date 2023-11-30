#include <Arduino.h>
#include <drivers/usart/usart_sync.h>
#include "../config.h"

/**
 * @brief initialize the serial ports
 */
void serial_init();

//
// helper macros
//

#if ENABLE_UI == 1
inline void SCREEN_PUTCHAR(char c)
{
    usart_sync_putc(SCREEN_SERIAL_PERIPHERAL, c);
}

inline void SCREEN_WRITE(const char *str)
{
    usart_sync_write(SCREEN_SERIAL_PERIPHERAL, str);
}
#endif // ENABLE_UI

inline void HOST_PUTCHAR(char c)
{
    usart_sync_putc(HOST_SERIAL_PERIPHERAL, c);
}

inline void HOST_WRITE(const char *str)
{
    usart_sync_write(HOST_SERIAL_PERIPHERAL, str);
}
