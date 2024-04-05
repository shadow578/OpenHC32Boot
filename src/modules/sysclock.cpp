#include "sysclock.h"
#include "../config.h"
#include <hc32_ddl.h>

namespace sysclock
{ 
  void _apply(const bool restore)
  {
    // SDIO seemingly requires HCLK and EXCLK to be 2:1
    // so set up the clock divider accordingly
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

  void reset()
  {
    #if SKIP_CLOCK_RESTORE != 1
      _apply(true);
    #endif
  }
} // namespace sysclock
