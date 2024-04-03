/**
 * hard fault handler taken from framework-arduino-hc32f46x 1.1.0:
 */
#include "fault_handler.h"
#include "../config.h"

#if ENABLE_FAULT_HANDLER == 1
 
/** 
 * hard fault handler based on:
 * - https://blog.feabhas.com/2018/09/updated-developing-a-generic-hard-fault-handler-for-arm-cortex-m3-cortex-m4-using-gcc/
 * - https://www.keil.com/appnotes/files/apnt209.pdf
 */
#include <hc32_ddl.h>
#include <cstdio>

#define panic_begin()
#define panic_printf(msg, ...) printf(msg, ##__VA_ARGS__)
#define panic_end()                     \
    {                                   \
    __BKPT(0);                          \
    __NVIC_SystemReset();               \
    }

typedef union hardfault_stack_frame_t
{
    struct
    {
        uint32_t r0;
        uint32_t r1;
        uint32_t r2;
        uint32_t r3;
        uint32_t r12;
        uint32_t lr;
        uint32_t pc;
        uint32_t psr;
    };
    uint32_t raw[8];
} hardfault_stack_frame_t;

void fault_handler::init()
{
    // enable cpu traps:
    // - divide by zero
    // - unaligned access
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk; //| SCB_CCR_UNALIGN_TRP_Msk;
}

/**
 * @brief print hardfault stack frame to panic output
 */
void print_stack_frame(hardfault_stack_frame_t *stack_frame)
{
    panic_printf("R0 = 0x%08lx\n", stack_frame->r0);
    panic_printf("R1 = 0x%08lx\n", stack_frame->r1);
    panic_printf("R2 = 0x%08lx\n", stack_frame->r2);
    panic_printf("R3 = 0x%08lx\n", stack_frame->r3);
    panic_printf("R12 = 0x%08lx\n", stack_frame->r12);
    panic_printf("LR = 0x%08lx\n", stack_frame->lr);
    panic_printf("PC = 0x%08lx\n", stack_frame->pc);
    panic_printf("PSR = 0x%08lx\n", stack_frame->psr);
}

/**
 * @brief hard fault handler in C, called by assembly wrapper
 */
extern "C" void HardFault_Handler_C(hardfault_stack_frame_t *stack_frame, uint32_t lr_value)
{
    // prepare panic message formatting
    panic_begin();

    // print panic message:
    // - header
    panic_printf("\n\n*** HARDFAULT ***\n");

    // - fault status registers
    panic_printf("- FSR / FAR:\n");
    panic_printf("SCB->HFSR = 0x%08lx\n", SCB->HFSR);
    panic_printf("SCB->CFSR = 0x%08lx\n", SCB->CFSR);
    panic_printf("SCB->DFSR = 0x%08lx\n", SCB->DFSR);
    panic_printf("SCB->AFSR = 0x%08lx\n", SCB->AFSR);

    if ((SCB->CFSR & SCB_CFSR_MMARVALID_Msk) != 0)
    {
        panic_printf("SCB->MMFAR = 0x%08lx\n", SCB->MMFAR);
    }

    if ((SCB->CFSR & SCB_CFSR_BFARVALID_Msk) != 0)
    {
        panic_printf("SCB->BFAR = 0x%08lx\n", SCB->BFAR);
    }

    // - stack frame
    panic_printf("- Stack frame:\n");
    print_stack_frame(stack_frame);

    // - misc
    //  * LR value
    panic_printf("- Misc:\n");
    panic_printf("LR = 0x%08lx\n", lr_value);

    // - footer
    panic_printf("***\n\n");

    // end panic message and halt
    panic_end();
}

/**
 * @brief hard fault handler wrapper in assembly
 */
extern "C" __attribute__((naked)) void HardFault_Handler(void)
{
    asm volatile(
        " tst lr, #4            \n" // check if we're using the MSP or PSP
        " ite eq                \n" // if equal, we're using the MSP
        " mrseq r0, msp         \n" //  * using MSP, load MSP into r0
        " mrsne r0, psp         \n" //  * using PSP, load PSP into r0
        " mov r1, lr            \n" // load LR into r1
        " b HardFault_Handler_C \n" // call handler in C
    );
}

#else
  void fault_handler::init() {}
#endif // ENABLE_FAULT_HANDLER == 1
