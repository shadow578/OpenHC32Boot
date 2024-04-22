#pragma once
#include <stdint.h>

namespace flash::write_protect
{
  /**
   * @brief enable write protection for the specified flash region
   */
  void enable();

  /**
   * @brief disable write protection
   */
  void disable();
} // namespace flash::write_protect
