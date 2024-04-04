#pragma once
#include <hc32_ddl.h>

namespace delay 
{
  /**
   * @brief Delay for a number of milliseconds
   * @param ms the number of milliseconds to delay
   */
  inline void ms(const uint32_t ms) { Ddl_Delay1ms(ms); }

  /**
   * @brief Delay for a number of microseconds
   * @param us the number of microseconds to delay
   */
  inline void us(const uint32_t us) { Ddl_Delay1us(us); }
} // namespace delay
