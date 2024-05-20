//
// OpenHC32Boot size-concious feature profile, removing debugging and verbosity to save space.
// this Profile is recommended for production use.
//
#pragma once
#include "../../config_options.h"
#include "../board.h"

// don't log the update metadata
#ifndef LOG_METADATA
  #define LOG_METADATA 0
#endif

// log info and error messages
#ifndef LOG_LEVEL
  #define LOG_LEVEL LOG_LEVEL_INFO
#endif

// disable the fault handler, removing the fault handler code
#ifndef ENABLE_FAULT_HANDLER
  #define ENABLE_FAULT_HANDLER 0
#endif

// small config takes < 16KB of flash, so
// the app can start at 0x4000
#ifndef APP_BASE_ADDRESS
  #define APP_BASE_ADDRESS 0x4000
#endif

// everything else matches the full feature profile
#include "full.h"
