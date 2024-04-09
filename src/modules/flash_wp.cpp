#include "flash_wp.h"
#include "../config.h"
#include "flash.h"
#include <hc32_ddl.h>

namespace flash::write_protect
{
  void set_protection_window(const uint32_t window_start, const uint32_t window_end)
  {
    EFM_Unlock();
    M4_EFM->FPMTSW_f.FPMTSW = window_start;
    M4_EFM->FPMTEW_f.FPMTEW = window_end;
    EFM_Lock();
  }

  void enable()
  {
    // according to the reference manual, section 7.6.7 "FLASH Wipe, Programming Window Protection"
    // the protection window defines the area of flash that is NOT protected, e.g. the area that can be erased or written to
    constexpr uint32_t start_address = (APP_BASE_ADDRESS - 1) & 0x000FFFFF;
    constexpr uint32_t end_address = (get_flash_size() - 1) & 0x000FFFFF;
    set_protection_window(start_address, end_address);
  }

  void disable()
  {
    // if window start address == window end address, the write protection is disabled
    set_protection_window(0, 0);
  }
} // namespace flash::write_protect
