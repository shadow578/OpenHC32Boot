#pragma once
#include "../../util.h"
#include "../../config.h"

namespace sdio
{
  struct pins_t
  {
    pins::pin_t d0;
    pins::pin_t d1;
    pins::pin_t d2;
    pins::pin_t d3;
    pins::pin_t clk;
    pins::pin_t cmd;
    pins::pin_t det;
  };
  
  /**
   * @brief SDIO pin assignment 
   */
  constexpr pins_t pins = SDIO_PINS;

  constexpr uint32_t read_timeout = 500;  // ms
  constexpr uint32_t write_timeout = 500; // ms
} // namespace sdio
