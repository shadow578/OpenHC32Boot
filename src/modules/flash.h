#pragma once
#include "sd.h"
#include "hash.h"
#include <algorithm>

namespace flash
{
  /**
   * @brief set to true to simulate the firmware update without writing to flash 
   */
  constexpr bool dry_run = true;

  constexpr uint32_t erase_sector_size = 8192; // 8Kb
  constexpr uint32_t file_buffer_size = 512;   // 512 bytes

  enum class update_stage
  {
    erase,
    write,
  };

  struct update_metadata
  {
    /**
     * @brief the size of the application in bytes
     */
    uint32_t app_size;
    
    /**
     * @brief the start address of the application in flash
     */
    hash::hash_t hash;

    #if STORE_UPDATE_METADATA == 1

      /**
       * @brief get the word count of the update metadata
       * @return the word count of the update metadata
       * @note equal to the length of the array returned by get_data() 
       */
      static constexpr uint32_t get_word_count()
      {
        uint32_t size = sizeof(update_metadata);

        // round up to the nearest word
        if (size % 4 != 0)
        {
          size += 4 - (size % 4);
        }

        // convert to word count
        return size / 4;
      }

      /**
       * @brief get the start address of the update metadata in flash
       * @param flash_end_address the end address of the flash
       * @return the start address of the update metadata in flash
       */
      static constexpr uint32_t get_start_address(const uint32_t flash_end_address)
      {
        return flash_end_address - (get_word_count() * 4);
      }

      /**
       * @brief get the update metadata as an array of words of length get_word_count()
       * @return the update metadata as an array of words
       * @note the array is padded to the nearest word
       * @note the array is dynamically allocated and must be freed by the caller
       */
      const uint32_t *get_data() const
      {
        const uint32_t *thisData = reinterpret_cast<const uint32_t *>(this);
        uint32_t *data = new uint32_t[get_word_count()];

        // copy the metadata to the data array
        std::copy(thisData, thisData + get_word_count(), data);

        return data;
      }

      /**
       * @brief get the current update metadata stored in flash
       * @param address start address of the metadata in flash
       * @return the current update metadata stored in flash or nullptr if the metadata is invalid 
       */
      static const update_metadata *get_current(const uint32_t address)
      {
        // get the metadata from the flash
        const uint32_t *metadata = reinterpret_cast<const uint32_t *>(address);

        // check if the metadata is valid
        if (metadata[0] == 0xFFFFFFFF)
        {
          return nullptr;
        }

        return reinterpret_cast<const update_metadata *>(metadata);
      }
  
    #endif // STORE_UPDATE_METADATA == 1
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

  /**
   * @brief get the total flash size of the MCU (including bootloader)
   * @note equal to the largest flash address + 1 
   */
  inline uint32_t get_flash_size()
  {
    #if FLASH_SIZE == 256
      return 0x40000;
    #elif FLASH_SIZE == 512
      return 0x80000;
    #else
      #error "Invalid FLASH_SIZE"
    #endif
  }
} // namespace flash
