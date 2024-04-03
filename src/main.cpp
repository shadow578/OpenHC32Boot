#include <hc32_ddl.h>
#include <cstdio>
#include "modules.h"

void on_progress(const flash::update_stage stage, const int done, const int total)
{
  printf("%s : %d of %d", stage == flash::update_stage::erase ? "erase" : "write", done, total);
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
  printf("OpenHC32Boot\n");

  // get the firmware file
  printf("checking %s\n", FIRMWARE_UPDATE_FILE);
  FIL file;
  if (sd::get_update_file(file, FIRMWARE_UPDATE_FILE))
  {
    // got the file, get metadata then flash
    flash::update_metadata metadata = {
      .app_size = f_size(&file),
      //TODO .hash = 
    };

    if(!flash::apply_firmware_update(file, APP_BASE_ADDRESS, metadata, &on_progress))
    {
      printf("update failed\n");
    }
    else
    {
      printf("update applied\n");
    }

    // close the file
    sd::close_update_file(file, FIRMWARE_UPDATE_FILE);
  }

  // jump to the application
  printf("jump to %08lx\n", APP_BASE_ADDRESS);
  leap::jump(APP_BASE_ADDRESS);
}
