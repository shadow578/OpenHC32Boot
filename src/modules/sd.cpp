#include "sd.h"
#include "log.h"
#include "../config.h"

namespace sd 
{
  #define HAS_METADATA_HASH (METADATA_HASH != HASH_NONE)

  /**
   * @brief FatFS file system object
   */
  FATFS fs;

  bool get_metadata(FIL *file, flash::update_metadata &metadata)
  {
    // get file size
    metadata.app_size = f_size(file);

    #if HAS_METADATA_HASH
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
    #endif // HAS_METADATA_HASH

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

    bool read_metadata = false;
    for(;;)
    {
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
      const DWORD file_size = f_size(&file);
      if (file_size == 0)
      {
        return false;
      }

      // get metadata
      if (!read_metadata)
      {
        if (!get_metadata(&file, metadata)) 
        {
          return false;
        }
        
        #if HAS_METADATA_HASH
          // open file again, but don't read metadata
          // this is (more or less) equal to f_rewind(), but 
          // with reduced memory usage
          #if SKIP_FILE_CLEANUP != 1
            f_close(&file);
          #endif

          read_metadata = true;
          continue;
        #endif
      }

      // done if the file size matches the metadata
      // since we may have re-opened the file, it's better to check 
      // the file size didn't change
      return metadata.app_size == file_size;
    }
  }

  void close_update_file(FIL &file, const char *path)
  {
    #if SKIP_FILE_CLEANUP != 1
      // close the file
      FRESULT res = f_close(&file);
      if (res != FR_OK)
      {
        logging::error("f_close() err=");
        logging::error(res, 10);
        logging::error("\n");
  
        // don't care for error here
      }
  
      #if DELETE_FIRMWARE_UPDATE_FILE == 1
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
    #endif // SKIP_FILE_CLEANUP != 1
  }
} // namespace sd
