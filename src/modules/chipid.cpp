#include "chipid.h"
#include <hc32_ddl.h>
#include "log.h"
#include "../util.h"

namespace chipid
{
  /**
   * @brief print the ARM Cortex-M CPUID 
   */
  void print_cpuid()
  {
    logging::log("CPUID=");
    logging::log(SCB->CPUID, 16);
    logging::log("\n");
  }

  /**
   * @brief print the EFM unique ID
   */
  void print_efm_uid()
  {
    const stc_efm_unique_id_t uid = EFM_ReadUID();
    logging::log("UID=");
    logging::log(uid.uniqueID1, 16);
    logging::log(" ");
    logging::log(uid.uniqueID2, 16);
    logging::log(" ");
    logging::log(uid.uniqueID3, 16);
    logging::log("\n");
  }

  /**
   * @brief print the system clock frequency 
   */
  void print_system_clock()
  {
    stc_clk_freq_t clk;
    CLK_GetClockFreq(&clk);
    logging::log("SYS@");
    logging::log(clk.sysclkFreq / 1000000, 10);
    logging::log("MHz, ");

    logging::log("HCLK@");
    logging::log(clk.hclkFreq / 1000000, 10);
    logging::log("MHz\n");
  }

  /**
   * @brief print the chip variant
   */
  void print_variant()
  {
    logging::log("CHIP=");
    switch (variant::get_variant())
    {
    case variant::HC32F460C:
      logging::log(STRINGIFY(HC32F460C));
      break;
    case variant::HC32F460E:
      logging::log(STRINGIFY(HC32F460E));
      break;
    case variant::UNKNOWN:
      logging::log(STRINGIFY(UNKNOWN));
      break;
    }
    logging::log("\n");
  }

  void print()
  {
    print_variant();
    print_cpuid();
    print_efm_uid();
    print_system_clock();
  }
} // namespace cpuid
