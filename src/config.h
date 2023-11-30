#pragma once
#include "configuration.h"
#include "boards/board.h"

#ifndef BOOTLOADER_VERSION
#define BOOTLOADER_VERSION "none"
#endif

/**
 * @brief bootloader welcome message
 */
#define BOOTLOADER_WELCOME_MESSAGE "OpenHC32Boot " BOOTLOADER_VERSION "\non " BOARD_NAME "\n\n"
