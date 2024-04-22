#include "sysclock.h"
#include "../config.h"
#include <hc32_ddl.h>

namespace sysclock
{ 
  void _apply(const bool restore)
  {
    // when setting the clock configuration: 
    // setup EXCLK clock according to HC32F460 Reference Manual, Section 4.4 "Working Clock Specification", Note 2:
    // "HCLK frequency: EXCLK frequency = 2:1, 4:1, 8:1, 16:1, 32:1"
    // PCLKx are also set up according to the same section, with 1:1 ratio.
    //
    // when restoring the clock configuration:
    // set all clocks to 1:1 ratio, according to Section 4.11.21 "CMU Clock Divider Configuration Register (CMU_SCFGR)"
    stc_clk_sysclk_cfg_t clock_divider_config = {
        .enHclkDiv = ClkSysclkDiv1,                             // HCLK  = 8 MHz (CPU)
        .enExclkDiv = restore ? ClkSysclkDiv1 : ClkSysclkDiv2,  // EXCLK = 8 / 4 MHz (SDIO)
        .enPclk0Div = ClkSysclkDiv1,                            // PCLK0 = 8 MHz (Timer6)
        .enPclk1Div = ClkSysclkDiv1,                            // PCLK1 = 8 MHz (USART, SPI, I2S, Timer0, TimerA)
        .enPclk2Div = ClkSysclkDiv1,                            // PCLK2 = 8 MHz (ADC)
        .enPclk3Div = ClkSysclkDiv1,                            // PCLK3 = 8 MHz (I2C, WDT)
        .enPclk4Div = ClkSysclkDiv1,                            // PCLK4 = 8 MHz (ADC ctl)
    };
    CLK_SysClkConfig(&clock_divider_config);
  }

  void apply()
  {
    _apply(false);
  }

  void restore()
  {
    #if SKIP_CLOCK_RESTORE != 1
      _apply(true);
    #endif
  }
} // namespace sysclock
