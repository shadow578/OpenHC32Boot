#pragma once
#include <source/pff.h>
#include "sd/sdio.h"
#include "flash.h"

namespace sd 
{
  /**
   * @brief open the update file at the given path
   * @param metadata metadata object
   * @param path path to the file
   * @return true if the file was opened successfully
   * 
   * @note assumes only one file is opened at a time
   */
  bool get_update_file(flash::update_metadata &metadata, const char *path);
} // namespace sd
