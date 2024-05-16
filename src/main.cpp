#include <hc32_ddl.h>
#include "modules.h"
#include "checks/runtime.h"

void on_progress(const flash::update_stage stage, const int done, const int total)
{
  // build update status string
  char status_str[
    5 +   // "erase" or "write"
    2 +   // ": "
    10 +  // done, base 10 -> max. 10 characters
    4 +   // " of "
    10 +  // total, base 10 -> max. 10 characters
    1     // null terminator
  ];

  strcpy(status_str, stage == flash::update_stage::erase ? "erase" : "write");
  strcat(status_str, ": ");
  logging::formatters::format_number(status_str + strlen(status_str), done, 10);
  strcat(status_str, " of ");
  logging::formatters::format_number(status_str + strlen(status_str), total, 10);

  // print the status to the host serial only
  // screen already shows the progress bar with the status message
  #if HAS_SERIAL(HOST_SERIAL)
    hostSerial.write(status_str);
    hostSerial.write("\n");
  #endif
  
  // print the status to the screen
  screen.showProgress(done, total, status_str);
  screen.flush();
}

int main()
{
  #if WAIT_FOR_DEBUGGER == 1
    while ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0)
    {
      __NOP();
    }
  #endif

  runtime_checks::run();

  // initialize system
  fault_handler::init();
  sysclock::apply();
  compat::apply();
  
  // initialize serial and ui
  #if HAS_SERIAL(HOST_SERIAL) 
    hostSerial.init(HOST_SERIAL_BAUD);
  #endif

  screen.init();

  #if ENABLE_BOOTLOADER_PROTECTION == 1
    // initialize protection of the bootloader region
    mpu::init(true, true, false);
    mpu::enable_region(0, mpu::region::build<
      LD_FLASH_START, 
      mpu::region::get_size(APP_BASE_ADDRESS - LD_FLASH_START), 
      mpu::permissions::get(mpu::permissions::READ_ONLY, mpu::permissions::READ_ONLY), 
      true>());
  #endif

  #if ENABLE_FLASH_WRITE_PROTECTION == 1
    // enable flash write protection
    flash::write_protect::enable();
  #endif

  // print hello message
  logging::log("OpenHC32Boot " BOOTLOADER_VERSION "\n");
  beep::beep(100);

  #if PRINT_CHIPID == 1
    chipid::print();
  #endif

  // get the firmware file
  logging::log("checking ");
  logging::log(FIRMWARE_UPDATE_FILE);
  logging::log("\n");
  
  flash::update_metadata metadata;
  if (sd::get_update_file(metadata, FIRMWARE_UPDATE_FILE))
  {
    // print new firmware metadata to info
    metadata.log("update");

    #if STORE_UPDATE_METADATA == 1
      const flash::update_metadata *stored_metadata = flash::update_metadata::get_stored();
      stored_metadata->log("flash");

      // check if we've already flashed this firmware
      if (metadata.equals(stored_metadata))
      {
        logging::log("update skipped\n");
      }
      else
    #endif

    {
      // apply the update
      if(!flash::apply_firmware_update(APP_BASE_ADDRESS, metadata, &on_progress))
      {
        logging::error("update failed\n");
        beep::beep(500, 999);
        ASSERT(false, "update failed");
      }
      else
      {
        logging::log("update applied\n");
      }
    }
  }

  // log application jump address
  logging::log("jumping to app @ ");
  logging::log(APP_BASE_ADDRESS, 16);
  logging::log("\n");

  // run pre-checks on the application
  if (!leap::pre_check(APP_BASE_ADDRESS))
  {
    logging::log("pre-check fail! skip jump\n");
    beep::beep(250, 999);
    ASSERT(false, "pre-check fail");
  }

  // deinitialize serial to prevent interference with the application
  #if HAS_SERIAL(HOST_SERIAL)
    hostSerial.deinit();
  #endif
  #if HAS_SERIAL(SCREEN_SERIAL)
    screenSerial.deinit();
  #endif

  // restore the clock configuration
  sysclock::restore();

  // jump to the application
  leap::jump(APP_BASE_ADDRESS);
}
