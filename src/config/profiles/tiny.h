//
// OpenHC32Boot size-optimized feature profile, sacrificing features for size
//
#pragma once
#include "../../config_options.h"

// disable the screen
// this removes the screen driver code
#ifndef SCREEN_DRIVER
  #define SCREEN_DRIVER SCREEN_NONE
#endif

#include "../board.h"

// don't delete the firmware update file after flashing it
// this way, fatfs compiles in read-only mode
#ifndef DELETE_FIRMWARE_UPDATE_FILE
  #define DELETE_FIRMWARE_UPDATE_FILE 0
#endif

// use CRC32 for metadata hash
// CRC32 driver is smaller than the SHA256 one
// selecting NONE would save even more space, but then the chip
// may re-flash repeatedly when the firmware is already up-to-date
#ifndef METADATA_HASH
  #define METADATA_HASH HASH_CRC32
#endif

// log nothing but mandatory messages
#ifndef LOG_LEVEL
  #define LOG_LEVEL LOG_LEVEL_NONE
#endif

// don't print chip ID
#ifndef PRINT_CHIPID
  #define PRINT_CHIPID 0
#endif

// disable the fault handler, removing the fault handler code
#ifndef ENABLE_FAULT_HANDLER
  #define ENABLE_FAULT_HANDLER 0
#endif

// disable bootloader protection, removing
// the MPU code
#ifndef ENABLE_BOOTLOADER_PROTECTION
  #define ENABLE_BOOTLOADER_PROTECTION 0
#endif

// run no pre-jump checks
#ifndef PRE_CHECK_LEVEL
  #define PRE_CHECK_LEVEL PRE_CHECK_MINIMAL
#endif

// enable all space-saving skips
#ifndef SKIP_USART_DEINIT
  #define SKIP_USART_DEINIT 1
#endif
#ifndef SKIP_FILE_CLEANUP
  #define SKIP_FILE_CLEANUP 1
#endif
#ifndef SKIP_CLOCK_RESTORE
  #define SKIP_CLOCK_RESTORE 1
#endif

// disable the beeper
#ifdef BEEPER_PIN
  #undef BEEPER_PIN
#endif

// use 1-bit SDIO mode
// this saves a few bytes of flash
//#ifdef SDIO_PINS
//  constexpr gpio::pin_t _sdio_pins[] = SDIO_PINS
//  #undef SDIO_PINS
//  #define SDIO_PINS { _sdio_pins[0], _sdio_pins[1], _sdio_pins[2], _sdio_pins[3]}
//#endif

// tiny config takes < 16KB of flash, so
// the app can start at 0x4000
#ifndef APP_BASE_ADDRESS
  #define APP_BASE_ADDRESS 0x4000
#endif

// everything else matches the full feature profile
#include "full.h"
