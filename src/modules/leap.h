#pragma once
#include <stdint.h>

namespace leap
{
  /**
   * @brief run prelaunch checks on the application
   * @param app_base_address the base address of the application, starting with the vector table
   * @return true if the application is valid and can be jumped to
   * @note call before serial deinitialization
   */
  bool pre_check(const uint32_t app_base_address);

  /**
   * @brief jump to the application code
   * @param app_base_address the base address of the application, starting with the vector table
   */
  void jump(const uint32_t app_base_address);
} // namespace leap