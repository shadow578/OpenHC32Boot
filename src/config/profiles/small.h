//
// OpenHC32Boot size-concious feature profile, sacrificing some extra features for size
// but less aggressive than the tiny profile
//
#pragma once
#include "../../config_options.h"
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

// log nothing but error messages
#ifndef LOG_LEVEL
  #define LOG_LEVEL LOG_LEVEL_ERROR
#endif

// don't print CPUID
#ifndef PRINT_CPUID
  #define PRINT_CPUID 0
#endif

// disable the fault handler, removing the fault handler code
#ifndef ENABLE_FAULT_HANDLER
  #define ENABLE_FAULT_HANDLER 0
#endif

// run only basic pre-jump checks
#ifndef PRE_CHECK_LEVEL
  #define PRE_CHECK_LEVEL PRE_CHECK_EXTENDED
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

// small config takes < 16KB of flash, so
// the app can start at 0x4000
#ifndef APP_BASE_ADDRESS
  #define APP_BASE_ADDRESS 0x4000
#endif

// everything else matches the full feature profile
#include "full.h"
