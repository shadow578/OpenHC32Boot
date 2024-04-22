//
// Voxelab Aquila V1.0.0/V1.0.1/V1.0.2/V1.0.3 (HC32F460) as found in the Voxelab Aquila X2 and C2
//
#pragma once
#include "../../config_options.h"

// Voxelab X2 uses DWIN screen, enable defaults
#include "shared/dwin.h"

// Voxelab V101 uses the HC32F460C variant
#ifndef CHIPID_VARIANT_OVERRIDE
  #define CHIPID_VARIANT_OVERRIDE chipid::variant::HC32F460C
#endif

// Serial ports
#ifndef HOST_SERIAL_TX
  #define HOST_SERIAL_TX gpio::PA9
#endif
#ifndef SCREEN_SERIAL_TX
  #define SCREEN_SERIAL_TX gpio::PC0
#endif

// Beeper pin
#ifndef BEEPER_PIN
  #define BEEPER_PIN gpio::PB13
#endif

// SDIO pins
#ifndef SDIO_PINS
  #define SDIO_PINS { gpio::PC12, gpio::PD2, gpio::PA10, gpio::PC8, gpio::PC9, gpio::PC10, gpio::PC11 }
#endif

// Voxelab Aquila X2 bootloader disables JTDO, JTDI, and NJTRST
#ifndef COMPAT_DISABLE_DEBUG_PORT
  #define COMPAT_DISABLE_DEBUG_PORT (COMPAT_DBG_JTDO | COMPAT_DBG_JTDI | COMPAT_DBG_NJTRST)
#endif
