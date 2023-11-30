#include <drivers/usart/usart_sync.h>
#include "serial/serial.h"
#include "ui/ui.h"

#if !defined(__GNUC__)
#error "only GCC is supported"
#endif

/**
 * @brief assume pointers passed to _write are already null-terminated
 * @note speeds up printing, but could cause the (last) character to be dropped
 */
#define ASSUME_NULL_TERMINATED 1

#if ASSUME_NULL_TERMINATED == 0
#define PRINTF_BUFFER_SIZE 256
char printf_buffer[PRINTF_BUFFER_SIZE];
#endif

/**
 * @brief implementation of _write that redirects everything to the host serial(s)
 * @param file file descriptor. don't care
 * @param ptr pointer to the data to write
 * @param len length of the data to write
 * @return number of bytes written
 */
extern "C" int _write(int file, char *ptr, int len)
{
#if ASSUME_NULL_TERMINATED == 1
    // ensure last character is a null terminator
    ptr[len] = '\0';

    // print to target(s)
    HOST_WRITE(ptr);
    UI::print(ptr);
#else // ASSUME_NULL_TERMINATED == 0
    int printed = 0;
    while (printed < len)
    {
        // limit length to buffer size
        int to_print = min(len - printed, PRINTF_BUFFER_SIZE - 1);

        // copy data to buffer
        memcpy(printf_buffer, ptr + printed, to_print);

        // ensure last character is a null terminator
        printf_buffer[to_print] = '\0';

        // print to target(s)
        HOST_WRITE(printf_buffer);
        UI::print(printf_buffer);

        // increment printed count
        printed += to_print;
    }
#endif

    return len;
}

/**
 * @brief implementation of _isatty that always returns 1
 * @param file file descriptor. don't care
 * @return everything is a tty. there are no files to be had
 */
extern "C" int _isatty(int file)
{
    return 1;
}
