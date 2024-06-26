//
// OpenHC32Boot full feature profile.
// NOT recommended for production use, as it mostly includes debugging features.
//
#pragma once
#include "../../config_options.h"
#include "../board.h"

// Host Serial port uses USART2 @ 115200 baud
// only if the board defines a HOST_SERIAL_TX pin
#ifdef HOST_SERIAL_TX
  #ifndef HOST_SERIAL
    #define HOST_SERIAL 2
  #endif
  #ifndef HOST_SERIAL_BAUD
    #define HOST_SERIAL_BAUD 115200
  #endif
#endif

// screen serial uses USART1 for DWIN screens
#if IS_SCREEN(SCREEN_DWIN)
  #ifndef SCREEN_SERIAL
    #define SCREEN_SERIAL 1
  #endif
#endif

// SDIO uses SDIOC1
#ifndef SDIO_PERIPHERAL
  #define SDIO_PERIPHERAL 1
#endif

// default to a firmware file located at the root of the SD card 
// and named FIRMWARE.BIN
#ifndef FIRMWARE_UPDATE_FILE
  #define FIRMWARE_UPDATE_FILE "FIRMWARE.BIN"
#endif

// store last update metadata in flash
#ifndef STORE_UPDATE_METADATA
  #define STORE_UPDATE_METADATA 1
#endif

// use SHA256 for metadata hash
#ifndef METADATA_HASH
  #define METADATA_HASH HASH_SHA256
#endif

// log the update metadata
#ifndef LOG_METADATA
  #define LOG_METADATA 1
#endif

// default to logging everything
#ifndef LOG_LEVEL
  #define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

// enable printing chip ID
#ifndef PRINT_CHIPID
  #define PRINT_CHIPID 1
#endif

// enable the fault handler
#ifndef ENABLE_FAULT_HANDLER
  #define ENABLE_FAULT_HANDLER 1
#endif

// enable protection of the bootloader
#ifndef ENABLE_BOOTLOADER_PROTECTION
  #define ENABLE_BOOTLOADER_PROTECTION 1
#endif

// enable write-protection of the bootloader region
#ifndef ENABLE_FLASH_WRITE_PROTECTION
  #define ENABLE_FLASH_WRITE_PROTECTION 1
#endif

// run full pre-jump checks
#ifndef PRE_CHECK_LEVEL
  #define PRE_CHECK_LEVEL PRE_CHECK_FULL
#endif

// skip nothing
#ifndef SKIP_USART_DEINIT
  #define SKIP_USART_DEINIT 0
#endif
#ifndef SKIP_CLOCK_RESTORE
  #define SKIP_CLOCK_RESTORE 0
#endif

// dont wait for a debugger
#ifndef WAIT_FOR_DEBUGGER
  #define WAIT_FOR_DEBUGGER 0
#endif

// disable DDL debugging mode
#ifdef __DEBUG
  #warning "consider disabling __DEBUG for production"
#endif

// don't use variant auto-detection if the board defines a variant
#ifndef CHIPID_VARIANT_OVERRIDE_ENABLE
  #ifdef CHIPID_VARIANT_OVERRIDE
    #define CHIPID_VARIANT_OVERRIDE_ENABLE 1
  #else
    #define CHIPID_VARIANT_OVERRIDE_ENABLE 0
  #endif
#endif

// full config will take about 22KB of flash, so 
// the app can start at 0x6000
#ifndef APP_BASE_ADDRESS
  #define APP_BASE_ADDRESS 0x6000ul
#endif
