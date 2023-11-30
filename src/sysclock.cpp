// TODO: file added as a quick fix, needs some more work...
//  for whatever reason, the SDIOC does not read correctly from the SD card with the default clock setting (8MHz MRC)
//  or maybe there is something wrong in the way the clock is reset by the core...
//  anyway, this is a quick fix to get the SD card working.
//  a permanent solution should also handle if the HRC is not 16MHz tho... also, it should probably not clock the cpu at 200MHz...

/**
 * HC32f460 system clock configuration
 */
#include <core_hooks.h>
#include <core_debug.h>
#include <drivers/sysclock/sysclock_util.h>

// get BOARD_XTAL_FREQUENCY from configuration / pins
// #define BOARD_XTAL_FREQUENCY 8000000

void core_hook_sysclock_init()
{
    // set wait cycles, as we are about to switch to 200 MHz HCLK
    sysclock_configure_flash_wait_cycles();
    sysclock_configure_sram_wait_cycles();

    // configure MPLLp to 200 MHz output, with different settings depending on XTAL availability
    // TODO do not use xtal, bc HRC is always available

    // HRC (16 MHz)
    // - M = 1   => 16 MHz / 1 = 16 MHz
    // - N = 21  => 16 MHz * 21 = 336 MHz
    // - P = 2   => 336 MHz / 2 = 168 MHz (sysclk)
    // - Q,R = 4 => 336 MHz / 4 = 84 MHz (dont care)
    stc_clk_mpll_cfg_t pllConf = {
        .PllpDiv = 2u, // P
        .PllqDiv = 4u, // Q
        .PllrDiv = 4u, // R
        .plln = 21u,   // N
        .pllmDiv = 1u, // M
    };
    sysclock_configure_hrc();
    sysclock_configure_mpll(ClkPllSrcHRC, &pllConf);

    // HRC could have been configured by ICG to 20 MHz
    // TODO: handle this gracefully
    if (1UL != (HRC_FREQ_MON() & 1UL))
    {
        panic("HRC is not 16 MHz");
    }

    // setup clock divisors for sysclk = 168 MHz:
    // note: PCLK1 is used for step+temp timers, and need to be kept at 50 MHz (until there is a better solution)
    stc_clk_sysclk_cfg_t sysClkConf = {
        .enHclkDiv = ClkSysclkDiv1,  // HCLK  = 168 MHz (CPU)
        .enExclkDiv = ClkSysclkDiv2, // EXCLK = 84 MHz (SDIO)
        .enPclk0Div = ClkSysclkDiv1, // PCLK0 = 168 MHz (Timer6 (not used))
        .enPclk1Div = ClkSysclkDiv4, // PCLK1 = 42 MHz (USART, SPI, I2S, Timer0 (step+temp), TimerA (Servo))
        .enPclk2Div = ClkSysclkDiv4, // PCLK2 = 42 MHz (ADC)
        .enPclk3Div = ClkSysclkDiv4, // PCLK3 = 42 MHz (I2C, WDT)
        .enPclk4Div = ClkSysclkDiv2, // PCLK4 = 84 MHz (ADC ctl)
    };
    sysclock_set_clock_dividers(&sysClkConf);

    // set power mode
#define POWER_MODE_SYSTEM_CLOCK 168000000 // 168 MHz
    power_mode_update_pre(POWER_MODE_SYSTEM_CLOCK);

    // switch to MPLL as sysclk source
    CLK_SetSysClkSource(CLKSysSrcMPLL);

    // set power mode
    power_mode_update_post(POWER_MODE_SYSTEM_CLOCK);
}
