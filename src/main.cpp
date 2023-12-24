#include <Arduino.h>
#include "serial/serial.h"
#include "ui/ui.h"
#include "update/update.h"
#include "fprotect/fprotect.h"
#include "leap/leap.h"
#include "config.h"

#include "iictest.h"

/**
 * @brief update firmware from sd card file
 * @return true if a update was applied, false otherwise
 * @note if firmware flashing fails, this function will loop forever
 * @note on bootloader updates, the user is asked to confirm the update
 * @note errors are printed using printf
 */
bool update_firmware()
{
    printf("update from " FIRMWARE_UPDATE_FILE_PATH "\n");

    // prepare the update
    FIL updateFile;
    if (!prepare_firmware_update(FIRMWARE_UPDATE_FILE_PATH, updateFile))
    {
        printf("no update\n");
        return false;
    }

    // apply the update
    bool ok = apply_firmware_update(updateFile, APP_BASE_ADDRESS);
    if (ok)
    {
        printf("updated success\n");
    }
    else
    {
        printf("update failed\n");
    }

    // end the update
    end_firmware_update(updateFile);

    // loop if update failed
    if (!ok)
    {
        while (true)
        {
            delay(1000);
        }
    }
    return ok;
}

void setup()
{
#if BOOT_DELAY > 0
    // add boot delay
    for (int i = 0; i < BOOT_DELAY; i++)
    {
        delay(1000);
    }
#endif

    // initialize serial
    serial_init();

    // initialize screen UI
    // after this, all printf's are also sent to the screen
    UI::init();

    // print bootloader and system info
    stc_efm_unique_id_t uid = EFM_ReadUID();
    printf(BOOTLOADER_WELCOME_MESSAGE);
    printf("CPUID=%08lx\n", SCB->CPUID);
    printf("UID=%08lx %08lx %08lx\n", uid.uniqueID1, uid.uniqueID2, uid.uniqueID3);

    iic_test_entry();

    NVIC_SystemReset();
}

void loop() {}
