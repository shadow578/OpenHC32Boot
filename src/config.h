#pragma once
#include "config_options.h"

// host serial port
// possible values: [ -1, 1, 2, 3, 4 ]
#define HOST_SERIAL 2
#define HOST_SERIAL_TX gpio::PA9
#define HOST_SERIAL_BAUD 115200

// screen implementation to use
// possible values: [ NONE, DWIN ]
#define SCREEN_DRIVER SCREEN_DWIN

// DWIN screen serial port
#if IS_SCREEN(SCREEN_DWIN)
  #define SCREEN_SERIAL 1
  #define SCREEN_SERIAL_TX gpio::PC0

  // DWIN screen rx pin.
  // if not defined, the screen module will use delay-based communication
  //#define SCREEN_SERIAL_RX gpio::PC1

  // DWIN screen orientation
  // possible values: [ portrait, landscape, portrait_inverted, landscape_inverted]
  #define SCREEN_ORIENTATION portrait

  // DWIM screen dimensions
  // format: { <width>, <height> }
  #define SCREEN_DIMENSIONS { 272, 480 }
#endif

// SDIO pin assignment. Bus width is selected accordingly.
// format: 
// - 1-bit bus width: { <CLK>, <CMD>, <DET>, <D0> }
// - 4-bit bus width: { <CLK>, <CMD>, <DET>, <D0>, <D1>, <D2>, <D3> }
// - 8-bit bus width: { <CLK>, <CMD>, <DET>, <D0>, <D1>, <D2>, <D3>, <D4>, <D5>, <D6>, <D7> }
#define SDIO_PINS { gpio::PC12, gpio::PD2, gpio::PA10, gpio::PC8, gpio::PC9, gpio::PC10, gpio::PC11 }

// SDIO Peripheral
// one of [ 1, 2 ]
#define SDIO_PERIPHERAL 1

// delete the firmware update file after flashing?
// possible values: [ 0, 1 ]
#define DELETE_FIRMEWARE_UPDATE_FILE 1

// store last update metadata in flash
#define STORE_UPDATE_METADATA 1

// hash to include in update metadata
// possible values: [ NONE, CRC32, SHA256 ]
#define METADATA_HASH HASH_SHA256

// path of the firmware update file
#define FIRMWARE_UPDATE_FILE "update.bin"

// base address of the application
#define APP_BASE_ADDRESS 0xC000ul

// enable fault handler
#define ENABLE_FAULT_HANDLER 1

// logging level
// possible values: [ DEBUG, INFO, ERROR, OFF ]
#define LOG_LEVEL LOG_LEVEL_DEBUG

// print the CPUID information
#define PRINT_CPUID 1

// beeper pin assignment
// if not defined, the beeper is disabled
// otherwise, beeper will be used to indicate errors
#define BEEPER_PIN gpio::PB13

// enable protection of the bootloader region using the MPU
// possible values: [ 0, 1 ]
#define ENABLE_BOOTLOADER_PROTECTION 1

// skip deinitialization of the USART peripheral on boot and before jumping to the application
// may cause issues with the application, so enable with caution
#define SKIP_USART_DEINIT 0

// skip calls to f_close and f_unmount for files.
// - in read-only mode, this should be safe, but still may cause issues
// - in read-write mode, this may changes to files to not be saved
// enable with caution
#define SKIP_FILE_CLEANUP 0
