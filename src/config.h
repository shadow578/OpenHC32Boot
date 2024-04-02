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

// SDIO pin assignment
// format: { <D0>, <D1>, <D2>, <D3>, <CLK>, <CMD>, <DET> }
#define SDIO_PINS { PC8, PC9, PC10, PC11, PC12, PD2, PA10 }

// SDIO Peripheral
// one of [ 1, 2 ]
#define SDIO_PERIPHERAL 1

// delete the firmware update file after flashing?
// possible values: [ 0, 1 ]
#define DELETE_FIRMEWARE_UPDATE_FILE 1
