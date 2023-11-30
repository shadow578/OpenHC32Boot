#include "leap.h"
#include <core_debug.h>

/**
 * partial vector table
 */
typedef struct vector_table_t
{
    uint32_t stack_top;
    void (*reset_handler)(void);
} vector_table_t;

/**
 * @brief pointer to the application vector table
 */
#define APP_VECTOR_TABLE ((vector_table_t *)APP_BASE_ADDRESS)

void goto_app()
{
    printf("goto_app at " STRINGIFY(APP_BASE_ADDRESS) "\n");

    // get application reset vector
    void (*app_reset_handler)(void) = APP_VECTOR_TABLE->reset_handler;

    // check if application reset handler is valid
    uint32_t app_reset_handler_address = (uint32_t)app_reset_handler;
    if (app_reset_handler == nullptr                    // nullpointer
        || app_reset_handler_address == 0x0             // "
        || app_reset_handler_address == 0xFFFFFFFF      // "
        || app_reset_handler_address < APP_BASE_ADDRESS // in bootloader area...
    )
    {
        printf("invalid app reset handler @ 0x%08lx\nabort boot", app_reset_handler_address);
        fflush(stdout);
        return;
    }

    // update stack pointer
    // TODO: ref only updated MSP if running in privileged mode
    __set_MSP(APP_VECTOR_TABLE->stack_top);

    // set vector table offset
    SCB->VTOR = APP_BASE_ADDRESS;

    // jump to application reset handler
    //printf("leap of faith, landing in 0x%08lx loving embrace\n", (uint32_t)app_reset_handler);
    fflush(stdout);
    app_reset_handler();
}
