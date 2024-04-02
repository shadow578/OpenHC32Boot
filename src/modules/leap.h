#pragma once
#include <stdint.h>

namespace leap
{
  /**
   * @brief jump to the application code
   * @param app_base_address the base address of the application, starting with the vector table
   */
  bool jump(const uint32_t app_base_address);
} // namespace leap