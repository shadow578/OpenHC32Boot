#include <hc32_ddl.h>
#include <cstdio>
#include "modules.h"

void on_progress(const flash::update_stage stage, const int done, const int total)
{
  logging::info(stage == flash::update_stage::erase ? "erase" : "write");
  logging::info(": ");
  logging::info(done, 10);
  logging::info(" of ");
  logging::info(total, 10);
  logging::info("\n");
  screen.showProgress((done * 100) / total);
  screen.flush();
}

int main()
{
  // initialize modules
  fault_handler::init();
  hostSerial.init(HOST_SERIAL_BAUD);
  screen.init();

  // print hello message
  logging::log("OpenHC32Boot " BOOTLOADER_VERSION "\n");

  #if PRINT_CPUID == 1
    cpuid::print();
  #endif

  // get the firmware file
  logging::log("checking ");
  logging::log(FIRMWARE_UPDATE_FILE);
  logging::log("\n");
  
  FIL file;
  flash::update_metadata metadata;
  if (sd::get_update_file(file, metadata, FIRMWARE_UPDATE_FILE))
  {
    // got the file, apply the update
    if(!flash::apply_firmware_update(file, APP_BASE_ADDRESS, metadata, &on_progress))
    {
      logging::error("update failed\n");
    }
    else
    {
      logging::log("update applied\n");
    }

    // close the file
    sd::close_update_file(file, FIRMWARE_UPDATE_FILE);
  }

  // jump to the application
  logging::log("jumping to app @ ");
  logging::log(APP_BASE_ADDRESS, 16);
  logging::log("\n");
  leap::jump(APP_BASE_ADDRESS);
}
