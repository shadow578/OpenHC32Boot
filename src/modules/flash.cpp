#include "flash.h"

/**
 * @brief EFM write timeout
 */
constexpr uint32_t efm_timeout = 0x1000;

/**
 * @brief mirror of EFM_SingleProgramRB, modified for batch programming
 * @param start the start address to write to
 * @param data the data to write
 * @param len the length of the data
 * @return the result of the operation [Ok, Error, ErrorTimeout]
 */
en_result_t EFM_SingleProgramRBBatch(const uint32_t start, const uint32_t *data, const uint32_t len)
{
  // clear error flags
  EFM_ClearFlag(EFM_FLAG_WRPERR | EFM_FLAG_PEPRTERR | EFM_FLAG_PGSZERR |
                  EFM_FLAG_PGMISMTCH | EFM_FLAG_EOP | EFM_FLAG_COLERR);

  // disable cache
  const uint32_t cache = M4_EFM->FRMC_f.CACHE;
  M4_EFM->FRMC_f.CACHE = Disable;

  // enable programming
  EFM_ErasePgmCmd(Enable);

  // select single-program mode with read-back
  M4_EFM->FWMC_f.PEMOD = EFM_MODE_SINGLEPROGRAMRB;

  en_result_t rc = Ok;
  for(int i = 0; i < len; i++)
  {
    // write word
    *(uint32_t *)(start + i * 4) = data[i];

    // wait for operation to complete
    uint32_t timeout = 0;
    while (M4_EFM->FSR_f.RDY != 1)
    {
      if (timeout++ > efm_timeout)
      {
        rc = ErrorTimeout;
        goto end_write_loop; // break out of outer loop
      }
    }

    // check readback flag
    if (M4_EFM->FSR_f.PGMISMTCH)
    {
      rc = Error;
      break;
    }

    // clear EOP flag
    EFM_ClearFlag(EFM_FLAG_EOP);
  }
  end_write_loop:

  // set flash to read-only mode
  M4_EFM->FWMC_f.PEMOD = EFM_MODE_READONLY;
  EFM_ErasePgmCmd(Disable);

  // restore cache mode
  M4_EFM->FRMC_f.CACHE = cache;

  return rc;
}

namespace flash
{
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
      printf("erase sector %ld @0x%08lx\n", sector, sector_address);

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
      progress(update_stage::erase, sector, total_sector_count);
    }

    return true;
  }

  /**
   * @brief write the firmware update to the flash
   * @param file firmware binary file to read the update from
   * @param start the start address to write the update to
   * @param end the end address to write the update to
   * @param progress callback function to report the write progress
   * @return true if the write was successful
   * @note assumes the flash is unlocked
   */
  bool write(FIL *file, const uint32_t start, const uint32_t end, const progress_callback progress)
  {
    const DWORD total_bytes = end - start;
    for(DWORD total_bytes_written = 0;;)
    {
      // read the next block
      UINT bytes_read = 0;
      const FRESULT res = f_read(file, buffer, file_buffer_size, &bytes_read);
      if (res != FR_OK)
      {
        printf("f_read() failed: %d\n", res);
        return false;
      }

      // check for end of file
      if (bytes_read == 0)
      {
        break;
      }

      // pad the buffer with 0xff if not aligned to 32-bit word
      while ((bytes_read % 4) != 0)
      {
        buffer[bytes_read++] = 0xff;
      }

      // prepare start address for this block
      const uint32_t block_address = start + total_bytes_written;

      // EFM_SingleProgram requires a uint32_t buffer, so
      // convert everything accordingly
      const uint32_t *buffer32 = reinterpret_cast<uint32_t *>(buffer);
      const uint32_t words_to_write = bytes_read / 4;

      // write the buffer to the flash
      printf("write %ld bytes @ 0x%08lx\n", bytes_read, block_address);
      en_result_t rc = Ok;
      
      if (!dry_run)
      {
        EFM_SingleProgramRBBatch(block_address, buffer32, words_to_write);
      }

      if (rc != Ok)
      {
        printf("EFM_SingleProgramRBBatch() rc=%d\n", rc);
        return false;
      }

      // verify write
      // printf("verify %ld bytes @ 0x%08lx\n", bytes_read, block_address);
      if (std::equal(buffer32, buffer32 + words_to_write, reinterpret_cast<uint32_t *>(block_address)))
      {
        printf("verify failed\n");
        return false;
      }

      // increment total bytes written
      total_bytes_written += bytes_read;

      // report progress
      progress(update_stage::write, total_bytes_written, total_bytes);
    }

    return true;
  }


  bool apply_firmware_update(FIL &file, const uint32_t app_base_address, const progress_callback progress)
  {

  }
} // namespace flash
