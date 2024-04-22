#include "mpu.h"
#include "log.h"
#include "assert.h"
#include <hc32_ddl.h>

// all devices in the HC32Fxxx series should have an MPU
static_assert(__MPU_PRESENT == 1, "MPU is not available for this device?!");

/**
 * @brief enter critical section for MPU configuration.
 * @param fn The function to execute in the critical section.
 * @note no interrupts and disable mpu
 */
#define MPU_CRITICAL_SECTION(fn)                                                                                       \
  {                                                                                                                    \
    const bool irqon = !__get_PRIMASK();                                                                               \
    __disable_irq();                                                                                                   \
    uint32_t mpu_ctrl = MPU->CTRL;                                                                                     \
    MPU->CTRL = mpu_ctrl & ~MPU_CTRL_ENABLE_Msk;                                                                       \
    __DSB();                                                                                                           \
    {                                                                                                                  \
      fn;                                                                                                              \
    }                                                                                                                  \
    __DSB();                                                                                                           \
    MPU->CTRL = mpu_ctrl;                                                                                              \
    if (irqon) __enable_irq();                                                                                         \
  }

namespace mpu
{
  void init(const bool enable, const bool enable_for_privileged, const bool enable_for_fault)
  {
    // validate that the REGION_COUNT is not greater than the hardware limit
    const uint8_t hw_region_count = (MPU->TYPE & MPU_TYPE_DREGION_Msk) >> MPU_TYPE_DREGION_Pos;
    ASSERT(region_count <= hw_region_count, "MPU region count exceeds hardware limit!");

    // enable the MPU
    uint32_t mpu_ctrl = 0;

    if (enable)
    {
      mpu_ctrl |= MPU_CTRL_ENABLE_Msk;
    }

    if (enable_for_privileged)
    {
      mpu_ctrl |= MPU_CTRL_PRIVDEFENA_Msk;
    }

    if (enable_for_fault)
    {
      mpu_ctrl |= MPU_CTRL_HFNMIENA_Msk;
    }

    MPU->CTRL = mpu_ctrl;
  }

  bool enable_region(const uint8_t region, const region::config &config)
  {
    // validate the region number
    ASSERT(region < region_count, "region number out of bounds!");

    // validate the base address alignment
    uint32_t base_address = config.base_address & MPU_RBAR_ADDR_Msk;
    ASSERT(base_address == config.base_address, "base address not aligned!");
    ASSERT((base_address & ((1 << (config.size)) - 1)) == 0, "base address not aligned to region size!");

    // prepare, then set the RBAR and RASR registers
    // note that RASR->SRD is not used here
    uint32_t rbar = base_address | MPU_RBAR_VALID_Msk | region;
    uint32_t rasr = (config.allow_execute ? 0 : MPU_RASR_XN_Msk)     // allow execution (e[X]ecute [N]ever)?
                    | (config.access_permissions << MPU_RASR_AP_Pos) // [A]ccess [P]ermissions
                    | config.tex_s_c_b                               // TEX + S + C + B
                    | ((config.size - 1) << MPU_RASR_SIZE_Pos)       // region size
                    | MPU_RASR_ENABLE_Msk;                           // enable the region

    MPU_CRITICAL_SECTION({
      MPU->RBAR = rbar;
      MPU->RASR = rasr;
    });
    return true;
  }

  void disable_region(const uint8_t region)
  {
    ASSERT(region < region_count, "region number out of bounds!");

    // clear the RBAR and RASR registers
    MPU_CRITICAL_SECTION({
      MPU->RBAR = MPU_RBAR_VALID_Msk | region;
      MPU->RASR = 0;
    });
  }
} // namespace mpu