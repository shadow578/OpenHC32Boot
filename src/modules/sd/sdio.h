#pragma once
#include "../../util.h"
#include "../../config.h"

namespace sdio
{
  constexpr gpio::pin_t __sdio_pins_arr[] = SDIO_PINS;
  constexpr int bus_width = countof(__sdio_pins_arr) - 3; // subtract CLK, CMD, DET

  struct pins_t
  {
    gpio::pin_t clk;
    gpio::pin_t cmd;
    gpio::pin_t det;
    gpio::pin_t dat[8];
  };
  
  /**
   * @brief SDIO pin assignment 
   */
  constexpr pins_t pins = SDIO_PINS;

  constexpr uint32_t read_timeout = 500;  // ms
  constexpr uint32_t write_timeout = 500; // ms
} // namespace sdio
