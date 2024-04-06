#include "compat.h"
#include "../config.h"
#include <hc32_ddl.h>

namespace compat
{
  void apply()
  {
    #if COMPAT_DISABLE_DEBUG_PORT != 0
      PORT_Unlock();
      M4_PORT->PSPCR &= static_cast<uint16_t>(~(COMPAT_DISABLE_DEBUG_PORT & 0x1Ful));
      PORT_Lock();
    #endif
  }
} // namespace compat
