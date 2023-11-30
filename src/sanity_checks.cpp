#include "config.h"
#include <Arduino.h>
#include "screen/screen.h"

// bootloader version must be defined
#ifndef BOOTLOADER_VERSION
#error "BOOTLOADER_VERSION not defined"
#endif

//
// board.h
//
// board name must be defined
#ifndef BOARD_NAME
#error "BOARD_NAME not defined"
#endif

#if (ENABLE_UI == 1)
// check screen serial connection
#ifndef SCREEN_SERIAL_PERIPHERAL
#error "SCREEN_SERIAL_PERIPHERAL not defined"
#endif

#ifndef SCREEN_SERIAL_BAUDRATE
#error "SCREEN_SERIAL_BAUDRATE not defined"
#endif

#ifndef SCREEN_SERIAL_TX_PIN
#error "SCREEN_SERIAL_TX_PIN not defined"
#endif

// check ui button
#ifndef UI_BUTTON_PIN
#error "UI_BUTTON_PIN not defined"
#endif

#ifndef UI_BUTTON_MODE
#error "UI_BUTTON_MODE not defined"
#endif

// UI_BUTTON_MODE must be either INPUT or INPUT_PULLUP
#if ((UI_BUTTON_MODE != INPUT) && (UI_BUTTON_MODE != INPUT_PULLUP))
#error "UI_BUTTON_MODE must be either INPUT or INPUT_PULLUP"
#endif
#endif // ENABLE_UI

// all SDIO pins must be defined
#if !defined(SDIO_D0_PIN) || !defined(SDIO_D1_PIN) || !defined(SDIO_D2_PIN) || !defined(SDIO_D3_PIN) || !defined(SDIO_CLK_PIN) || !defined(SDIO_CMD_PIN) || !defined(SDIO_DET_PIN)
#error "some SDIO pins are not defined"
#endif

//
// configuration.h
//
// firmware update file path must be defined
#ifndef FIRMWARE_UPDATE_FILE_PATH
#error "FIRMWARE_UPDATE_FILE_PATH not defined"
#endif

// APP_BASE_ADDRESS must be defined
#ifndef APP_BASE_ADDRESS
#error "APP_BASE_ADDRESS not defined"
#endif

// warn if UPDATE_DRY_RUN is enabled
#if (UPDATE_DRY_RUN == 1)
#warning "UPDATE_DRY_RUN is enabled"
#endif

// warn if boot delay is enabled
#if (BOOT_DELAY > 0)
#warning "BOOT_DELAY is enabled, bootup will be delayed"
#endif

// APP_BASE_ADDRESS must be aligned to 8K (erase block size) and 512 (VTO alignment requirement)
#if ((APP_BASE_ADDRESS % 8192) != 0)
#error "APP_BASE_ADDRESS must be aligned to 8K (erase block size)"
#endif

#if ((APP_BASE_ADDRESS % 512) != 0)
#error "APP_BASE_ADDRESS must be aligned to 512 (VTO alignment requirement)"
#endif

// APP_BASE_ADDRESS must be greater than bootloader base address
// (LD_FLASH_START is arg injected by core, equal to the start of the FLASH section)
#if (APP_BASE_ADDRESS <= LD_FLASH_START)
#error "APP_BASE_ADDRESS must be greater than bootloader base address"
#endif

#if ENABLE_FLASH_PROTECTION == 1

// FLASH_PROTECTION_START must be less than FLASH_PROTECTION_END
#if (FLASH_PROTECTION_START >= FLASH_PROTECTION_END)
#error "FLASH_PROTECTION_START must be less than FLASH_PROTECTION_END"
#endif

// FLASH_PROTECTION_END must be less than APP_BASE_ADDRESS
#if (FLASH_PROTECTION_END >= APP_BASE_ADDRESS)
#error "FLASH_PROTECTION_END must be less than APP_BASE_ADDRESS"
#endif

#else // ENABLE_FLASH_PROTECTION == 0
#warning "flash protection is disabled. this is not recommended"
#endif // ENABLE_FLASH_PROTECTION

#if ENABLE_UI == 1
// SCREEN_ORIENTATION must be defined
#ifndef SCREEN_ORIENTATION
#error "SCREEN_ORIENTATION not defined"
#endif

// SCREEN_ORIENTATION must be either PORTRAIT, LANDSCAPE, PORTRAIT_INVERTED or LANDSCAPE_INVERTED
#if ((SCREEN_ORIENTATION != PORTRAIT) && (SCREEN_ORIENTATION != LANDSCAPE) && (SCREEN_ORIENTATION != PORTRAIT_INVERTED) && (SCREEN_ORIENTATION != LANDSCAPE_INVERTED))
#error "SCREEN_ORIENTATION must be either PORTRAIT, LANDSCAPE, PORTRAIT_INVERTED or LANDSCAPE_INVERTED"
#endif

// SCREEN_WIDTH must be defined
#ifndef SCREEN_WIDTH
#error "SCREEN_WIDTH not defined"
#endif

// SCREEN_HEIGHT must be defined
#ifndef SCREEN_HEIGHT
#error "SCREEN_HEIGHT not defined"
#endif
#endif // ENABLE_UI
