#pragma once
#include "config_options.h"

// flash size
// possible values: [ 256, 512 ]
#define FLASH_SIZE 256

// host serial port
// possible values: [ -1, 1, 2, 3, 4 ]
#define HOST_SERIAL 2
#define HOST_SERIAL_TX pins::PA9
#define HOST_SERIAL_BAUD 115200

// screen implementation to use
// possible values: [ NONE, DWIN ]
#define SCREEN_DRIVER SCREEN_DWIN

// DWIN screen serial port
#if IS_SCREEN(SCREEN_DWIN)
  #define SCREEN_SERIAL 1
  #define SCREEN_SERIAL_TX pins::PC0

  // DWIN screen orientation
  // possible values: [ portrait, landscape, portrait_inverted, landscape_inverted]
  #define SCREEN_ORIENTATION portrait
#endif

// SDIO pin assignment
// format: { <D0>, <D1>, <D2>, <D3>, <CLK>, <CMD>, <DET> }
#define SDIO_PINS { pins::PC8, pins::PC9, pins::PC10, pins::PC11, pins::PC12, pins::PD2, pins::PA10 }

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
