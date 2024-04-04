#pragma once

namespace beep
{
  /**
   * @brief Beep the buzzer
   * @param duration the duration of the beep in milliseconds
   * @param repeat the number of times to repeat the beep
   */
  void beep(const uint32_t duration = 500, const uint32_t repeat = 1);
} // namespace beep
