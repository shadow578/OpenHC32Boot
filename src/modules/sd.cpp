#include "sd.h"
#include "../config.h"
#include <cstdio>

namespace sd 
{
  /**
   * @brief FatFS file system object
   */
  FATFS fs;

  bool get_update_file(FIL &file, const char *path)
  {
    // mount the file system
    FRESULT res = f_mount(&fs, "", /* mount now */ 1);
    if (res != FR_OK)
    {
      printf("f_mount() failed: %d\n", res);
      return false;
    }

    // try to open the file
    res = f_open(&file, path, FA_READ);
    if (res != FR_OK)
    {
      printf("f_open() failed: %d\n", res);
      return false;
    }

    // assume everything is ok
    // if the file is more than 0 bytes
    return f_size(&file) > 0;
  }

  void close_update_file(FIL &file, const char *path)
  {
    // close the file
    FRESULT res = f_close(&file);
    if (res != FR_OK)
    {
      printf("f_close() failed: %d\n", res);
      // don't care for error here
    }

    #if DELETE_FIRMEWARE_UPDATE_FILE == 1
      res = f_unlink(path);
      if (res != FR_OK)
      {
        printf("f_unlink() failed: %d\n", res);
        // don't care for error here
      }
    #endif

    // unmount the file system
    res = f_unmount("");
    if (res != FR_OK)
    {
      printf("f_unmount() failed: %d\n", res);
      // don't care for error here
    }
  }
} // namespace sd
