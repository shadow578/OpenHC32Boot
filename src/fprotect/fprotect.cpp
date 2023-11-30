#include "fprotect.h"
#if ENABLE_FLASH_PROTECTION == 1

#include <Arduino.h>
#include <hc32_ddl.h>

// TODO: protection violation interrupt does not work... why?
// constexpr IRQn_Type VIOLATION_IRQ = Int000_IRQn;
//
// void flash_protect_violation_irq()
// {
//     // flash protection was violated, print and loop forever
//     printf("flash protection violated\n");
//     while (true)
//     {
//         delay(1000);
//     }
// }

void flash_protect_enable()
{
    // unlock flash and enable
    EFM_Unlock();
    EFM_FlashCmd(Enable);

    // wait until ready flag is set
    while (Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        ;
    }

    // setup flash protection window
    stc_efm_win_protect_addr_t window = {
        .StartAddr = FLASH_PROTECTION_START,
        .EndAddr = FLASH_PROTECTION_END,
    };
    EFM_SetWinProtectAddr(window);

    // // enable flash protection violation interrupt
    // EFM_InterruptCmd(PgmErsErrInt, Enable);
    //
    // // set interrupt handler
    // stc_irq_regi_conf_t irqConf = {
    //     .enIntSrc = INT_EFM_PEERR,
    //     .enIRQn = VIOLATION_IRQ,
    //     .pfnCallback = flash_protect_violation_irq,
    // };
    // enIrqRegistration(&irqConf);
    // NVIC_ClearPendingIRQ(VIOLATION_IRQ);
    // NVIC_SetPriority(VIOLATION_IRQ, DDL_IRQ_PRIORITY_03);
    // NVIC_EnableIRQ(VIOLATION_IRQ);

    // re-lock flash
    EFM_Lock();
}

void flash_protect_disable_irq()
{
    // // disable interrupt
    // NVIC_DisableIRQ(VIOLATION_IRQ);
    // NVIC_ClearPendingIRQ(VIOLATION_IRQ);
    // enIrqResign(VIOLATION_IRQ);
}

#endif // ENABLE_FLASH_PROTECTION