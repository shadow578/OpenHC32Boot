#pragma once

namespace sysclock
{
  /**
   * @brief configure the system clock source
   */
  void apply();

  /**
   * @brief restore the system clock source to defaults
   */
  void restore();
} // namespace sysclock
