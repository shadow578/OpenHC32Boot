#include "cpuid.h"
#include <hc32_ddl.h>
#include "log.h"

namespace cpuid
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
  }
} // namespace cpuid
