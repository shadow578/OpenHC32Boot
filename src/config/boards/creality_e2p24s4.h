//
// Creality Ender 2 Pro v2.4.S4_170 (HC32f460KCTA)
//
#pragma once
#include "../../config_options.h"

// Ender 2 Pro uses DOGM screen, so disable screen
#ifndef SCREEN_DRIVER
  #define SCREEN_DRIVER SCREEN_NONE
#endif

// Ender 2 Pro uses the HC32F460C variant
#ifndef CHIPID_VARIANT_OVERRIDE
  #define CHIPID_VARIANT_OVERRIDE chipid::variant::HC32F460C
#endif

// Serial ports
#ifndef HOST_SERIAL_TX
  #define HOST_SERIAL_TX gpio::PA8
#endif

// Beeper pin
#ifndef BEEPER_PIN
  #define BEEPER_PIN gpio::PC6
#endif

// SDIO pins
#ifndef SDIO_PINS
  #define SDIO_PINS { gpio::PC12, gpio::PD2, gpio::PA10, gpio::PC8, gpio::PC9, gpio::PC10, gpio::PC11 }
#endif

// Ender 2 Pro bootloader disables JTDO, JTDI, and NJTRST
#ifndef COMPAT_DISABLE_DEBUG_PORT
  #define COMPAT_DISABLE_DEBUG_PORT (COMPAT_DBG_JTDO | COMPAT_DBG_JTDI | COMPAT_DBG_NJTRST)
#endif
