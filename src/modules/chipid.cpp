#include "chipid.h"
#include <hc32_ddl.h>
#include "log.h"

namespace chipid
{
  void print()
  {
    // CPUID
    logging::log("CPUID=");
    logging::log(SCB->CPUID, 16);
    logging::log("\n");

    // EFM unique ID
    const stc_efm_unique_id_t uid = EFM_ReadUID();
    logging::log("UID=");
    logging::log(uid.uniqueID1, 16);
    logging::log(" ");
    logging::log(uid.uniqueID2, 16);
    logging::log(" ");
    logging::log(uid.uniqueID3, 16);
    logging::log("\n");

    // system clock speed
    stc_clk_freq_t clk;
    CLK_GetClockFreq(&clk);
    logging::log("SYS@");
    logging::log(clk.sysclkFreq / 1000000, 10);
    logging::log("MHz\n");

    logging::log("HCLK@");
    logging::log(clk.hclkFreq / 1000000, 10);
    logging::log("MHz\n");
  }
} // namespace cpuid
