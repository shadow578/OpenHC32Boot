#pragma once
#include "../../util.h"
#include "../../config.h"

namespace sdio
{
  struct pins_t
  {
    gpio::pin_t d0;
    gpio::pin_t d1;
    gpio::pin_t d2;
    gpio::pin_t d3;
    gpio::pin_t clk;
    gpio::pin_t cmd;
    gpio::pin_t det;
  };
  
  /**
   * @brief SDIO pin assignment 
   */
  constexpr pins_t pins = SDIO_PINS;

  constexpr uint32_t read_timeout = 500;  // ms
  constexpr uint32_t write_timeout = 500; // ms
} // namespace sdio
