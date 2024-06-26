#include "flash.h"
#include <hc32_ddl.h>
#include "log.h"

namespace flash
{
  /**
   * @brief buffer used for reading from the firmware update file
   */
  BYTE buffer[file_buffer_size];

  /**
   * @brief erase the flash sectors in the given range
   * @param start the start address to erase
   * @param end the end address to erase
   * @param progress callback function to report the erase progress
   * @return true if the erase was successful
   * @note assumes the flash is unlocked 
   */
  bool erase(const uint32_t start, const uint32_t end, const progress_callback progress)
  {
    // get sectors
    const uint32_t start_sector = start / erase_sector_size; // inclusive
    const uint32_t end_sector = end / erase_sector_size;     // inclusive

    // calculate total sector count for progress calculation
    const uint32_t total_sector_count = (end_sector - start_sector) + 1;

    // erase sectors
    for (uint32_t sector = start_sector; sector <= end_sector; sector++)
    {
      // erase sector
      const uint32_t sector_address = sector * erase_sector_size;
      logging::debug("erase sector ");
      logging::debug(sector, 10);
      logging::debug(" @ 0x");
      logging::debug(sector_address, 16);
      logging::debug("\n");

      en_result_t rc = Ok;
      if (!dry_run)
      {
        rc = EFM_SectorErase(sector_address);
      }

      if (rc != Ok)
      {
        return false;
      }

      // update progress
      progress(update_stage::erase, sector - start_sector, total_sector_count);
    }

    return true;
  }

  /**
   * @brief write the data to the flash
   * @param data the data to write
   * @param start_address the start address to write the data to
   * @param words_to_write the number of words to write
   * @return true if the write was successful
   */
  bool write(const uint32_t start_address, const uint32_t *data, const uint32_t words_to_write)
  {
    // write the buffer to the flash
    logging::debug("write ");
    logging::debug(words_to_write, 10);
    logging::debug(" words @ 0x");
    logging::debug(start_address, 16);
    logging::debug("\n");

    // start address must be on a 32-bit boundary
    if (start_address % 4 != 0)
    {
      logging::error("start address misaligned\n");
      return false;
    }

    for(uint32_t i = 0, address = start_address; i < words_to_write; i++, address += 4)
    {
      en_result_t rc = Ok;
      if (!dry_run)
      {
        rc = EFM_SingleProgramRB(address, data[i]);
      }

      if (rc != Ok)
      {
        logging::error("EFM_SingleProgramRB() err=");
        logging::error(rc, 10);
        logging::error("\n");
        return false;
      }
    }

    // verify write
    // logging::debug("verify ");
    // logging::debug(words_to_write, 10);
    // logging::debug(" words @ 0x");
    // logging::debug(start_address, 16);
    // logging::debug("\n");
    if (!std::equal(data, data + words_to_write, reinterpret_cast<uint32_t *>(start_address)))
    {
      logging::debug("verify failed\n");
      return false;
    }

    return true;
  }

  /**
   * @brief write the firmware update to the flash
   * @param start the start address to write the update to
   * @param end the end address to write the update to
   * @param progress callback function to report the write progress
   * @return true if the write was successful
   * @note assumes the flash is unlocked
   */
  bool write_file(const uint32_t start, const uint32_t end, const progress_callback progress)
  {
    const DWORD total_bytes = end - start;
    bool did_pad = false;
    for(DWORD total_bytes_written = 0;;)
    {
      // read the next block
      UINT bytes_read = 0;
      const FRESULT res = pf_read(buffer, file_buffer_size, &bytes_read);
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

      // pad the buffer with 0xff if not aligned to 32-bit word
      // only allowed at the end of the file
      if((bytes_read % 4) != 0)
      {
        if (did_pad)
        {
          logging::error("attempted to pad data twice\n");
          return false;
        }

        while ((bytes_read % 4) != 0)
        {
          buffer[bytes_read++] = 0xff;
        }

        did_pad = true;
      }
      
      // prepare start address for this block
      const uint32_t block_address = start + total_bytes_written;

      // write the block to the flash
      if (!write(block_address, reinterpret_cast<uint32_t *>(buffer), bytes_read / 4))
      {
        return false;
      }

      // increment total bytes written
      total_bytes_written += bytes_read;

      // report progress
      progress(update_stage::write, total_bytes_written, total_bytes);
    }

    return true;
  }

  bool apply_firmware_update(const uint32_t app_base_address, const update_metadata &metadata, const progress_callback progress)
  {
    // calculate end addresses
    const uint32_t program_end_address = app_base_address + metadata.app_size;
    const uint32_t flash_end_address = get_flash_size() - 1;
    const uint32_t metadata_start_address = update_metadata::get_start_address(flash_end_address);

    // ensure the update fits in the flash
    #if STORE_UPDATE_METADATA == 1
      if (program_end_address >= metadata_start_address) // can only occupy up to the metadata
    #else
      if (program_end_address > flash_end_address) // can occupy the entire flash
    #endif
    {
      logging::error("update too large\n");
      return false;
    }

    // unlock and enable flash
    EFM_Unlock();
    EFM_FlashCmd(Enable);
    while (EFM_GetFlagStatus(EFM_FLAG_RDY) != Set) { /* nada */ }

    // disable interrupts while programming flash
    // noInterrupts();

    bool success = true;

    // erase the flash required for the application
    if (!erase(app_base_address, program_end_address, progress))
    {
      logging::error("app erase failed\n");
      success = false;
      goto cleanup; // cannot return directly because of cleanup
    }

    #if STORE_UPDATE_METADATA == 1
      // erase the flash required for the metadata
      // metadata is stored at the end of the flash
      if (!erase(metadata_start_address, flash_end_address, progress))
      {
        logging::error("metadata erase failed\n");
        success = false;
        goto cleanup; // cannot return directly because of cleanup
      }
    #endif

    // write the firmware update to the flash
    if (!write_file(app_base_address, program_end_address, progress))
    {
      logging::error("write app failed\n");
      success = false;
      goto cleanup; // cannot return directly because of cleanup
    }

    #if STORE_UPDATE_METADATA == 1
      // write the metadata
      if (!write(metadata_start_address, metadata.get_data(), metadata.get_word_count()))
      {
        logging::error("write metadata failed\n");
        success = false;
        goto cleanup; // cannot return directly because of cleanup
      }
    #endif

    cleanup:

    // re-enable interrupts
    // interrupts();

    // re-lock flash
    EFM_Lock();

    return success;
  }
} // namespace flash
