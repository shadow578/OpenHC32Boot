#include "sd.h"
#include "log.h"
#include "../config.h"
#include <cstdio>

namespace sd 
{
  /**
   * @brief FatFS file system object
   */
  FATFS fs;

  bool get_metadata(FIL *file, flash::update_metadata &metadata)
  {
    // get file size
    metadata.app_size = f_size(file);

    #if METADATA_HASH != HASH_NONE
      // start hash session
      if (!hash::start())
      {
        logging::error("hash::start() failed\n");
        return false;
      }

      // write data to hash
      constexpr size_t buffer_size = 512;
      uint8_t buffer[buffer_size];
      for(;;)
      {
        // read the next block
        UINT bytes_read = 0;
        const FRESULT res = f_read(file, buffer, buffer_size, &bytes_read);
        if (res != FR_OK)
        {
          logging::error("f_read() err=");
          logging::error(res, 10);
          logging::error("\n");
          return false;
        }

        // check for end of file
        if (bytes_read == 0)
        {
          break;
        }

        // write bytes to hash
        if (!hash::push_data(buffer, bytes_read))
        {
          logging::error("hash::push_data() failed\n");
          return false;
        }
      }

      // get the hash
      if (!hash::get_hash(metadata.hash))
      {
        logging::error("hash::get_hash() failed\n");
        return false;
      }
    #endif // METADATA_HASH != HASH_NONE

    return true;
  }

  bool get_update_file(FIL &file, flash::update_metadata &metadata, const char *path)
  {
    // mount the file system
    FRESULT res = f_mount(&fs, "", /* mount now */ 1);
    if (res != FR_OK)
    {
      logging::error("f_mount() err=");
      logging::error(res, 10);
      logging::error("\n");
      return false;
    }

    // try to open the file
    res = f_open(&file, path, FA_READ);
    if (res != FR_OK)
    {
      logging::error("f_open() err=");
      logging::error(res, 10);
      logging::error("\n");
      return false;
    }

    // assume everything is ok
    // if the file is more than 0 bytes
    if (f_size(&file) <= 0)
    {
      return false;
    }

    // get metadata
    return get_metadata(&file, metadata);
  }

  void close_update_file(FIL &file, const char *path)
  {
    // close the file
    FRESULT res = f_close(&file);
    if (res != FR_OK)
    {
      logging::error("f_close() err=");
      logging::error(res, 10);
      logging::error("\n");

      // don't care for error here
    }

    #if DELETE_FIRMEWARE_UPDATE_FILE == 1
      res = f_unlink(path);
      if (res != FR_OK)
      {
        logging::error("f_unlink() err=");
        logging::error(res, 10);
        logging::error("\n");

        // don't care for error here
      }
    #endif

    // unmount the file system
    res = f_unmount("");
    if (res != FR_OK)
    {
      logging::error("f_unmount() err=");
      logging::error(res, 10);
      logging::error("\n");

      // don't care for error here
    }
  }
} // namespace sd
