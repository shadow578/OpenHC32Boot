#pragma once
#include "../config.h"

namespace chipid
{
  namespace variant
  {
    enum variant_t
    {
      /**
       * @brief HC32F460, 256K Flash, 192K RAM
       */
      HC32F460C,

      /**
       * @brief HC32F460, 512K Flash, 192K RAM
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
    inline const variant_t get_variant()
    {
      #if CHIPID_VARIANT_OVERRIDE_ENABLE == 1 && defined(CHIPID_VARIANT_OVERRIDE)
        // use the override
        return CHIPID_VARIANT_OVERRIDE;
      #else
        // auto-detect the variant at runtime
        // on HC32F460, the EFM registers include a UNDOCUMENTED (!) register
        // 'FRANDS' that might be used to differentiate between the two variants.
        // on the 256K variant, this register seems to be constant 0x3fff.
        // i'm assuming the 512K variant has a different value here.
        //TODO: confirm FRANDS value for 512K variant
        const uint32_t frands = M4_EFM->FRANDS_f.FRANDS;
        if (frands == 0x3fff)
        {
          return HC32F460C;
        }
        else //if (frands == ???)
        {
          return HC32F460E;
        }

        return UNKNOWN;
      #endif
    }
  } // namespace variant

  /**
   * @brief print the chip ID information
   */
  void print();
} // namespace chipid
