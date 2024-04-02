#pragma once
#include "config_options.h"

// host serial port
// possible values: [ -1, 1, 2, 3, 4 ]
#define HOST_SERIAL 2
#define HOST_SERIAL_TX PA9
#define HOST_SERIAL_BAUD 115200

// screen implementation to use
// possible values: [ NONE, DWIN ]
#define SCREEN_DRIVER SCREEN_DWIN

// DWIN screen serial port
#if IS_SCREEN(SCREEN_DWIN)
  #define SCREEN_SERIAL 1
  #define SCREEN_SERIAL_TX PC0

  // DWIN screen orientation
  // possible values: [ portrait, landscape, portrait_inverted, landscape_inverted]
  #define SCREEN_ORIENTATION portrait
#endif
