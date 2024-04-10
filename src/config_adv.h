#pragma once
#include "config_options.h"

// host serial port
// possible values: [ -1, 1, 2, 3, 4 ]
//define HOST_SERIAL 2
//define HOST_SERIAL_TX gpio::PA9
//define HOST_SERIAL_BAUD 115200

// screen implementation to use
// possible values: [ NONE, DWIN ]
//define SCREEN_DRIVER SCREEN_DWIN

// DWIN screen serial port
#if IS_SCREEN(SCREEN_DWIN)
  //define SCREEN_SERIAL 1
  //define SCREEN_SERIAL_TX gpio::PC0

  // DWIN screen orientation
  // possible values: [ portrait, landscape, portrait_inverted, landscape_inverted]
  //define SCREEN_ORIENTATION portrait

  // DWIM screen dimensions
  // format: { <width>, <height> }
  //define SCREEN_DIMENSIONS { 272, 480 }
#endif

// SDIO pin assignment. Bus width is selected accordingly.
// format: 
// - 1-bit bus width: { <CLK>, <CMD>, <DET>, <D0> }
// - 4-bit bus width: { <CLK>, <CMD>, <DET>, <D0>, <D1>, <D2>, <D3> }
// - 8-bit bus width: { <CLK>, <CMD>, <DET>, <D0>, <D1>, <D2>, <D3>, <D4>, <D5>, <D6>, <D7> }
//define SDIO_PINS { gpio::PC12, gpio::PD2, gpio::PA10, gpio::PC8, gpio::PC9, gpio::PC10, gpio::PC11 }

// SDIO Peripheral
// one of [ 1, 2 ]
//define SDIO_PERIPHERAL 1

// delete the firmware update file after flashing?
// possible values: [ 0, 1 ]
//define DELETE_FIRMEWARE_UPDATE_FILE 0

// store last update metadata in flash
//define STORE_UPDATE_METADATA 1

// hash to include in update metadata
// possible values: [ NONE, CRC32, SHA256 ]
//define METADATA_HASH HASH_SHA256

// path of the firmware update file
//define FIRMWARE_UPDATE_FILE "firmware.bin"

// base address of the application
//define APP_BASE_ADDRESS 0xC000ul

// override the chip variant with CHIPID_VARIANT_OVERRIDE, disabling auto-detection
// possible values: [ 0, 1 ]
//define CHIPID_VARIANT_OVERRIDE_ENABLE 1

// chip variant override
// possible values: any in chipid::variant::
//define CHIPID_VARIANT_OVERRIDE chipid::variant::HC32F460C

// enable fault handler
//define ENABLE_FAULT_HANDLER 1

// logging level
// possible values: [ DEBUG, INFO, ERROR, OFF ]
//define LOG_LEVEL LOG_LEVEL_DEBUG

// print the chip ID information
//define PRINT_CHIPID 1

// beeper pin assignment
// if not defined, the beeper is disabled
// otherwise, beeper will be used to indicate errors
//define BEEPER_PIN gpio::PB13

// enable protection of the bootloader region using the MPU
// possible values: [ 0, 1 ]
//define ENABLE_BOOTLOADER_PROTECTION 1

// enable flash write protection for the bootloader region
// this is recommended in place of the MPU protection, as it also protects against sector erase
// possible values: [ 0, 1 ]
//define ENABLE_FLASH_WRITE_PROTECTION 1

// run pre-jump checks
// possible values: [ MINIMAL, EXTENDED, FULL ]
//define PRE_CHECK_LEVEL PRE_CHECK_FULL

// skip deinitialization of the USART peripheral on boot and before jumping to the application
// may cause issues with the application, so enable with caution
//define SKIP_USART_DEINIT 0

// skip calls to f_close and f_unmount for files.
// - in read-only mode, this should be safe, but still may cause issues
// - in read-write mode, this may changes to files to not be saved
// enable with caution
//define SKIP_FILE_CLEANUP 0

// skip restoring the clock configuration before jumping to the application
//define SKIP_CLOCK_RESTORE 0

// wait for a debugger to be attached before running the bootloader
//define WAIT_FOR_DEBUGGER 0

// disable debugging pins for compatibilty with firmware expecting other bootloaders
// combination of [ COMPAT_DBG_SWCLK, COMPAT_DBG_SWDIO, COMPAT_DBG_JTDO, COMPAT_DBG_JTDI, COMPAT_DBG_NJTRST ] (e.g. COMPAT_DBG_SWCLK | COMPAT_DBG_SWDIO)
// or preset, one of [ COMPAT_DBG_VOXELAB_AQUILA_X2 ]
// or undefined to keep the default configuration
//define COMPAT_DISABLE_DEBUG_PORT COMPAT_DBG_VOXELAB_AQUILA_X2
