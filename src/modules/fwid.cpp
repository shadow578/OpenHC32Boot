#include "fwid.h"
#include "log.h"
#include "../config.h"
#include "../util.h"

#if APP_BASE_ADDRESS == 0x2000
  #define APP_BASE_ADDRESS_STR "0x2000"
#elif APP_BASE_ADDRESS == 0x4000
  #define APP_BASE_ADDRESS_STR "0x4000"
#elif APP_BASE_ADDRESS == 0x6000
  #define APP_BASE_ADDRESS_STR "0x6000"
#elif APP_BASE_ADDRESS == 0x8000
  #define APP_BASE_ADDRESS_STR "0x8000"
#else
  #error "Invalid APP_BASE_ADDRESS for FWID"
#endif

void fwid::print()
{
  // "OpenHC32Boot A:[APP_BASE_ADDRESS] V:[VERSION] ", eg.: "OpenHC32Boot A:0x4000 V:1.0.0 "
  // Note: the 'check_fwid.py' script depends on this format, so it must be updated if this is changed
  logging::log("OpenHC32Boot A:" APP_BASE_ADDRESS_STR " V:" BOOTLOADER_VERSION "\n");
}
