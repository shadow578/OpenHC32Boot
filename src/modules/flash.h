#pragma once
#include "sd/fatfs/ff.h"
#include "hash.h"
#include "chipid.h"
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
   * @brief get the total flash size of the MCU (including bootloader)
   * @note equal to the largest flash address + 1 
   */
  inline const uint32_t get_flash_size()
  {
    switch (chipid::variant::get_variant())
    {
    case chipid::variant::HC32F460C:
      return 0x40000;
    case chipid::variant::HC32F460E:
      // according to HC32F460 reference manual, section 7.3 "Embedded FLASH", 
      // the last 32 bytes of flash (0x7FFE0 - 0x7FFFF) are reserved and cannot
      // be programmed.
      return 0x80000 - 32;
    default:
      return 0;
    }
  }

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
        uint32_t address = flash_end_address - (get_word_count() * 4);

        // address must be aligned to the nearest word
        // so round down to the nearest word
        address -= address % 4;
        
        return address;
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
       * @brief get the update metadata stored in flash
       * @return the update metadata stored in flash or nullptr if the metadata is invalid 
       */
      static const update_metadata *get_stored()
      {
        return reinterpret_cast<const update_metadata *>(get_start_address(get_flash_size() - 1));
      }
  
    #endif // STORE_UPDATE_METADATA == 1

    /**
     * @brief check if the stored metadata matches this metadata
     * @return true if the stored metadata matches
     * @note returns true if STORE_UPDATE_METADATA is disabled 
     */
    const bool matches_stored() const
    {
      #if STORE_UPDATE_METADATA == 1
        const uint32_t *this_data = this->get_data();
        const uint32_t *stored_data = get_stored()->get_data();
        return stored_data != nullptr && std::equal(this_data, this_data + get_word_count(), stored_data);
      #else
        return true;
      #endif
    }
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
