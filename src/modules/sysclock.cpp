#include "sysclock.h"
#include <hc32_ddl.h>
#include "assert.h"

namespace sysclock
{ 
  void apply()
  {
    // check that HRC is 16 MHz
    // it could have been configured by ICG to 20 MHz
    ASSERT((HRC_FREQ_MON() & 1UL) == 1, "HRC is not 16 MHz");

    // enable the HRC clock
    // if not already enabled
    CLK_HrcCmd(Enable);

    // configure MPLL to generate 168 MHz sysclk from 16 MHz HRC
    // - M = 1   => 16 MHz / 1 = 16 MHz
    // - N = 21  => 16 MHz * 21 = 336 MHz
    // - P = 2   => 336 MHz / 2 = 168 MHz (sysclk)
    // - Q,R = 4 => 336 MHz / 4 = 84 MHz (dont care)
    stc_clk_mpll_cfg_t pll_config = {
        .PllpDiv = 2u, // P
        .PllqDiv = 4u, // Q
        .PllrDiv = 4u, // R
        .plln = 21u,   // N
        .pllmDiv = 1u, // M
    };
    CLK_SetPllSource(ClkPllSrcHRC);
    CLK_MpllConfig(&pll_config);

    CLK_MpllCmd(Enable);
    while (CLK_GetFlagStatus(ClkFlagMPLLRdy) != Set) { /* nada*/ }

    // setup clock divisors for sysclk = 168 MHz:
    stc_clk_sysclk_cfg_t clock_divider_config = {
        .enHclkDiv = ClkSysclkDiv1,  // HCLK  = 168 MHz (CPU)
        .enExclkDiv = ClkSysclkDiv2, // EXCLK = 84 MHz (SDIO)
        .enPclk0Div = ClkSysclkDiv1, // PCLK0 = 168 MHz (Timer6)
        .enPclk1Div = ClkSysclkDiv4, // PCLK1 = 42 MHz (USART, SPI, I2S, Timer0, TimerA)
        .enPclk2Div = ClkSysclkDiv4, // PCLK2 = 42 MHz (ADC)
        .enPclk3Div = ClkSysclkDiv4, // PCLK3 = 42 MHz (I2C, WDT)
        .enPclk4Div = ClkSysclkDiv2, // PCLK4 = 84 MHz (ADC ctl)
    };
    CLK_SysClkConfig(&clock_divider_config);

    // set flash wait cycles
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_3);
    EFM_Lock();

    // switch to MPLL as sysclk source
    CLK_SetSysClkSource(CLKSysSrcMPLL);
  }

} // namespace sysclock
