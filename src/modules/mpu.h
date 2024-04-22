/**
 * MPU driver based on framework-arduino-hc32f46x MPU driver. 
 */

#pragma once
#include <stdint.h>

/**
 * @brief ARM Cortex-M4 MPU driver.
 * https://interrupt.memfault.com/blog/fix-bugs-and-secure-firmware-with-the-mpu#mpu-region-base-address-register
 */
namespace mpu
{
  /**
   * @brief The number of available MPU regions.
   */
  constexpr uint8_t region_count = 8;

  namespace permissions
  {
    enum permissions
    {
      /**
       * @brief disallow read and write access.
       */
      NO_ACCESS = 0,

      /**
       * @brief allow read access.
       */
      READ_ONLY = 1,

      /**
       * @brief allow read and write access.
       */
      READ_WRITE = 3
    };

    /**
     * @brief get mpu access permissions
     * @param privileged The privileged mode access permissions.
     * @param user The user mode access permissions.
     * @return The mpu access permissions (AP) register value
     */
    constexpr uint8_t get(const permissions privileged, const permissions user)
    {
      switch (privileged)
      {
      case permissions::NO_ACCESS:
        switch (user)
        {
        case permissions::NO_ACCESS:
          return 0; // AP[2:0] = 000
        case permissions::READ_ONLY:
        case permissions::READ_WRITE:
        default:
          break;
        }
        break;

      case permissions::READ_ONLY:
        switch (user)
        {
        case permissions::NO_ACCESS:
          return 5; // AP[2:0] = 101
        case permissions::READ_ONLY:
          return 6; // AP[2:0] = 110
        case permissions::READ_WRITE:
        default:
          break;
        }
        break;

      case permissions::READ_WRITE:
        switch (user)
        {
        case permissions::NO_ACCESS:
          return 1; // AP[2:0] = 001
        case permissions::READ_ONLY:
          return 2; // AP[2:0] = 010
        case permissions::READ_WRITE:
          return 3; // AP[2:0] = 011
        default:
          break;
        }
        break;
      }

      // unsupported access permission combination
      return 0xff;
    }
  } // namespace permissions

  namespace region
  {
    struct config
    {
      /**
       * @brief The base address of the region.
       * @note the base address must be aligned to the region size.
       * @note the lower 5 bits of the base address are masked, so minimum alignment is 32 bytes.
       */
      uint32_t base_address;

      /**
       * @brief The size of the region.
       * @note minimum size is 1
       * @note size in bytes is calculates using: 2^(size).
       * @example size = 8 -> 2^8 = 256 bytes.
       * @note MPU_RASR.SIZE = (size - 1)
       */
      uint8_t size;

      /**
       * @brief The access permissions of the region in privileged mode.
       */
      uint8_t access_permissions;
      
      /**
       * @brief TEX, S, C, B bits for the region.
       * @note already shifted to the correct position.
       */
      uint32_t tex_s_c_b;

      /**
       * @brief Allow execution in the region (XN bit)?
       */
      bool allow_execute;
    };

    /**
     * @brief Get the size of the region for use in config.size.
     * @param size_bytes The size of the region in bytes.
     * @param allow_overshoot Allow the region size to be larger than the requested size?
     *                        If false, the region size may be smaller than the requested size.
     * @return The size of the region in for use in config.size.
     */
    constexpr uint8_t get_size(const uint32_t size_bytes, const bool allow_overshoot = false)
    {
      // get the smallest power of 2 that is greater or equal to the size
      uint8_t size = 1;
      while((1ul << size) < size_bytes) {
        size++;
      }

      // if the size is larger than the requested size, reduce it by 1
      // unless overshoot is allowed
      if (!allow_overshoot) {
        if ((1ul << size) > size_bytes)
        {
          size--;
        }
      }

      return size;
    }

    /**
     * @brief get the [T]ype [E]xtension [M]ask and
     * [S]hareable, [C]acheable and [B]ufferable bits
     * based on the region the base address is in.
     */
    constexpr uint32_t get_tex_scb(const uint32_t base_address)
    {
      //constexpr uint32_t FLASH_REGION_START = 0x00000000;
      constexpr uint32_t SRAM_REGION_START = 0x1FFF8000;
      constexpr uint32_t PERIPH_REGION_START = 0x40008000;
      //constexpr uint32_t PERIPH_REGION_END = 0xFFFFFFFF;

      uint8_t tex_s_c_b = 0;
      if (/*address >= FLASH_REGION_START &&*/ base_address < SRAM_REGION_START)
      {
        // FLASH
        // normal memory, non-shareable, write-through
        // TEX = 0b000, C = 1, B = 0, S = 0
        tex_s_c_b = 0x2;
      }
      else if (/*address >= SRAM_REGION_START &&*/ base_address < PERIPH_REGION_START)
      {
        // SRAM
        // normal memory, shareable, write-through
        // TEX = 0b000, C = 1, B = 0, S = 1
        tex_s_c_b = 0x6;
      }
      else /*if (address >= PERIPH_REGION_START && address <= PERIPH_REGION_END)*/
      {
        // PERIPHERALS
        // device memory, non-cacheable, non-bufferable
        // TEX = 0b000, C = 0, B = 1, S = 1
        tex_s_c_b = 0x5;
      }

      // shift the TEX, S, C, B bits to the correct position
      return tex_s_c_b << MPU_RASR_B_Pos;
    }

    /**
     * @brief Build a region configuration.
     * @param base_address The base address of the region.
     * @param size The size of the region, as power of 2.
     * @param access_permissions The access permissions of the region in privileged mode.
     * @param allow_execute Allow execution in the region (XN bit)?
     * @return The region configuration.
    */
    template <uint32_t base_address, uint8_t size, uint8_t access_permissions, bool allow_execute>
    constexpr config build()
    {
      static_assert(size > 0, "region size must be greater than 0!");
      static_assert(access_permissions != 0xff, "unsupported access permissions!");

      static_assert((base_address & MPU_RBAR_ADDR_Msk) == base_address, "base address not aligned!");
      static_assert((base_address & ((1 << size) - 1)) == 0, "base address not aligned to region size!");

      constexpr uint32_t tex_s_c_b = get_tex_scb(base_address);
      static_assert(tex_s_c_b != 0, "unsupported region base address - TEX S C B fail!");

      return {
          .base_address = base_address,
          .size = size,
          .access_permissions = access_permissions,
          .tex_s_c_b = tex_s_c_b,
          .allow_execute = allow_execute};
    }
  }

  /**
   * @brief Initialize the MPU.
   * @param enable Enable the MPU (ENABLE).
   * @param enable_for_privileged Enable the MPU for privileged mode (PRIVDEFENA).
   * @param enable_for_fault Enable the MPU for fault handling (HFNMIENA).
   */
  void init(const bool enable = true, const bool enable_for_privileged = true, const bool enable_for_fault = false);

  /**
   * @brief Enable a region in the MPU.
   * @param region The region number
   * @param config The region configuration.
   * @return true if the region was enabled successfully, false otherwise.
   */
  bool enable_region(const uint8_t region, const region::config &config);

  /**
   * @brief Disable a region in the MPU.
   * @param region The region number
   */
  void disable_region(const uint8_t region);
}; // namespace mpu
