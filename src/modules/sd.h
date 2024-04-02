#pragma once
#include "sd/fatfs/ff.h"
#include "sd/sdio.h"

namespace sd 
{
  /**
   * @brief open the update file at the given path
   * @param file file object
   * @param path path to the file
   * @return true if the file was opened successfully
   * 
   * @note assumes only one file is opened at a time
   */
  bool get_update_file(FIL &file, const char *path);

  /**
   * @brief close the update file, deleting it if requested
   * @param file file object
   * @param path path to the file (needed for deletion)
   * @return true if the file was closed successfully
   *
   * @note assumes only one file is opened at a time
   */
  void close_update_file(FIL &file, const char *path);
} // namespace sd
