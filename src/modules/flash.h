#pragma once
#include <source/pff.h>
#include "flash_metadata.h"
#include <algorithm>

namespace flash
{
  /**
   * @brief set to true to simulate the firmware update without writing to flash 
   */
  constexpr bool dry_run = false;

  constexpr uint32_t erase_sector_size = 8192; // 8Kb
  constexpr uint32_t file_buffer_size = 512;   // 512 bytes

  static_assert(file_buffer_size % 4 == 0, "file buffer size must be aligned to 32-bit words");

  enum class update_stage
  {
    erase,
    write,
  };

  /**
   * @brief firmware update callback function
   * @param stage the current update stage
   * @param done the number of elements processed
   * @param total the total number of elements to process
   */
  typedef void (*progress_callback)(const update_stage stage, const int done, const int total);

  /**
   * @brief apply a firmware update from the given file
   * @param file firmware binary file to read the update from
   * @param app_base_address base address to write the firmware binary file to
   * @param metadata the update metadata
   * @param progress callback function to report the update progress
   * @return true if the firmware update was successful
   */
  bool apply_firmware_update(FIL &file, const uint32_t app_base_address, const update_metadata &metadata, const progress_callback progress);
} // namespace flash
