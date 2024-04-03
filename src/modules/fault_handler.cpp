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
#include "log.h"

#define LOG_REGISTER(message, register) logging::log(message "0x"); logging::log(register, 16); logging::log("\n");

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
    LOG_REGISTER("R0 = ", stack_frame->r0);
    LOG_REGISTER("R1 = ", stack_frame->r1);
    LOG_REGISTER("R2 = ", stack_frame->r2);
    LOG_REGISTER("R3 = ", stack_frame->r3);
    LOG_REGISTER("R12 = ", stack_frame->r12);
    LOG_REGISTER("LR = ", stack_frame->lr);
    LOG_REGISTER("PC = ", stack_frame->pc);
    LOG_REGISTER("PSR = ", stack_frame->psr);
}

/**
 * @brief hard fault handler in C, called by assembly wrapper
 */
extern "C" void HardFault_Handler_C(hardfault_stack_frame_t *stack_frame, uint32_t lr_value)
{
    // print panic message:
    // - header
    logging::log("\n\n*** HARDFAULT ***\n");

    // - fault status registers
    logging::log("- FSR / FAR:\n");
    LOG_REGISTER("SCB->HFSR = ", SCB->HFSR);
    LOG_REGISTER("SCB->CFSR = ", SCB->CFSR);
    LOG_REGISTER("SCB->DFSR = ", SCB->DFSR);
    LOG_REGISTER("SCB->AFSR = ", SCB->AFSR);

    if ((SCB->CFSR & SCB_CFSR_MMARVALID_Msk) != 0)
    {
        LOG_REGISTER("SCB->MMFAR = ", SCB->MMFAR);
    }

    if ((SCB->CFSR & SCB_CFSR_BFARVALID_Msk) != 0)
    {
        LOG_REGISTER("SCB->BFAR = ", SCB->BFAR);
    }

    // - stack frame
    logging::log("- Stack frame:\n");
    print_stack_frame(stack_frame);

    // - misc
    //  * LR value
    logging::log("- Misc:\n");
    LOG_REGISTER("LR = \n", lr_value);

    // - footer
    logging::log("***\n\n");

    // end panic message and halt
    __BKPT(0);
    __NVIC_SystemReset();
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
