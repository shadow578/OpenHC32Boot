#pragma once

//
// Board Registry
//

/**
 * @brief voxelab aquila V1.0.1 / V1.0.2 board
 * @note HC32F460 /w DWIN T5 LCD
 */
#define VOXELAB_AQUILA_V101 100

//
// import board configuration file
//
#include "../configuration.h"
#ifndef BOARD
#error "no BOARD is not defined"
#endif

#if BOARD == VOXELAB_AQUILA_V101
#include "voxelab_aquila_v101.h"
#else
#error "unknown board"
#endif
