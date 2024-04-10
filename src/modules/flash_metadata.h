#pragma once
#include "hash.h"
#include "chipid.h"
#include "log.h"
#include "../config.h"
#include <algorithm>

namespace flash
{
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
    
    #if METADATA_HASH != HASH_NONE
      /**
       * @brief the hash of the application
       */
      hash::hash_t hash;
    #endif

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
     * @brief check if if given metadata matches this metadata
     * @param other the metadata to compare to
     * @return true if the metadata matches, false otherwise
     */
    const bool equals(const update_metadata *other) const
    {
      const uint32_t *this_data = this->get_data();
      const uint32_t *other_data = other->get_data();
      return other_data != nullptr && std::equal(this_data, this_data + get_word_count(), other_data);
    }

    /**
     * @brief print update metadata to logging::info
     * @param description the description to print before the metadata
     */
    const void log(const char* description) const
    {
      #if LOG_METADATA == 1
        logging::info("Metadata for ");
        logging::info(description);
        logging::info("\n");

        // print app size
        logging::info("app_size: ");
        logging::info(app_size, 10);
        logging::info("Bytes\n");

        // print hash if enabled
        #if METADATA_HASH != HASH_NONE
          #if METADATA_HASH == HASH_CRC32
            logging::info("CRC32: ");
          #elif METADATA_HASH == HASH_SHA256
            logging::info("SHA256: ");
          #else
            #error "don't know how to print this hash"
          #endif

          // print as 4 / 32 two-digit hex numbers
          const uint8_t *hash_arr = reinterpret_cast<const uint8_t *>(&hash);
          for (size_t i = 0; i < sizeof(hash); i++)
          {
            logging::info(hash_arr[i], 16);
            logging::info(" ");
          }
          logging::info("\n");
        #endif
      #endif
    }
  };
} // namespace flash
