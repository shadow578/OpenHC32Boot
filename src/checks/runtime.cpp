#include "runtime.h"
#include "../modules/assert.h"
#include "../config.h"
#include <startup/ld_symbols.h>

namespace runtime_checks
{
  namespace app_after_etext
  {
    /**
     * @brief get the last used flash address of the bootloader 
     */
    inline uint32_t get_bootloader_end()
    {
      return reinterpret_cast<uint32_t>(&__etext_ret_ram) 
        + (reinterpret_cast<uint32_t>(&__data_end_ret_ram__) - reinterpret_cast<uint32_t>(&__data_start_ret_ram__)); 
    }

    /**
     * @brief check that the application base address is 
     * in the next erase sector after the bootloader's end of text 
     */
    bool check()
    {
      // get first address of the next erase sector after the bootloader
      // erase sector size is 8KB
      const uint32_t next_erase_sector_start = (get_bootloader_end() + 8192) & ~8191;

      // the app base address must be in the next erase sector
      return APP_BASE_ADDRESS >= next_erase_sector_start;
    }
  }

  void run()
  {
    ASSERT(app_after_etext::check(), "app base address is not in the next erase sector after the bootloader");
  }
} // namespace runtime_checks
