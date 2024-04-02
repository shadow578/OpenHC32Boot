#pragma once
#include <Arduino.h>
#include "../../util.h"
#include "../../config.h"

namespace sdio
{
  struct pins_t
  {
    gpio_pin_t d0;
    gpio_pin_t d1;
    gpio_pin_t d2;
    gpio_pin_t d3;
    gpio_pin_t clk;
    gpio_pin_t cmd;
    gpio_pin_t det;
  };
  
  /**
   * @brief SDIO pin assignment 
   */
  constexpr pins_t pins = SDIO_PINS;

  constexpr uint32_t read_timeout = 500;  // ms
  constexpr uint32_t write_timeout = 500; // ms
} // namespace sdio
