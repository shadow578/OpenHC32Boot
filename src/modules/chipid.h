#pragma once

namespace chipid
{
  namespace variant
  {
    enum variant_t
    {
      /**
       * @brief HC32F460, 256K Flash
       */
      HC32F460C,

      /**
       * @brief HC32F460, 512K Flash
       */
      HC32F460E,

      /**
       * @brief detection failed 
       */
      UNKNOWN,
    };

    /**
     * @brief get the variant of the chip
     * @return the variant of the chip  
     */
    constexpr variant_t get_variant()
    {
      // TODO: auto-detect variant at runtime
      constexpr uint32_t flash_size = (LD_FLASH_SIZE + LD_FLASH_START);
      if (flash_size == 0x80000)
      {
        return HC32F460E;
      }
      else if (flash_size == 0x40000)
      {
        return HC32F460C;
      }

      return UNKNOWN;
    }
  } // namespace variant

  /**
   * @brief print the chip ID information
   */
  void print();
} // namespace chipid
