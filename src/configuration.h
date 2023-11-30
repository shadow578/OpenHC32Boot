/**
 * Open HC32 Boot configuration file
 */
#pragma once

/**
 * mainboard to compile for
 * see boards/board.h for available boards
 */
#define BOARD VOXELAB_AQUILA_V101

/*
 * path to firmware update file on SD card
 */
#define FIRMWARE_UPDATE_FILE_PATH "/firmware.bin"

/**
 * delete the firmware update file after applying the update
 * (~ 3.3 KBytes)
 */
#define DELETE_FIRMEWARE_UPDATE_FILE 0

/**
 * application base address
 * must be aligned to 8Kb (erase sector size) and 512 bytes (VTO alignment requirement)
 */
#define APP_BASE_ADDRESS 0x18000

/**
 * enable dry-run mode for update operations
 * (with DRY_RUN enabled, no write operations are performed)
 */
#define UPDATE_DRY_RUN 0

/**
 * add a delay before starting the bootloader.
 * 0 to disable
 */
#define BOOT_DELAY 10 // seconds

/**
 * enable or disable flash write protection
 * (~ 100 Bytes)
 */
// TODO: currently not working...
#define ENABLE_FLASH_PROTECTION 0

#if ENABLE_FLASH_PROTECTION == 1
/**
 * start address of flash write protection area
 */
#define FLASH_PROTECTION_START 0x0000

/**
 * end address of flash write protection area
 */
#define FLASH_PROTECTION_END (APP_BASE_ADDRESS - 1)
#endif

/**
 * enable or disable the user interface
 * (~ 9 KBytes)
 */
#define ENABLE_UI 1

#if ENABLE_UI == 1
/**
 * screen orientation
 * one of: PORTRAIT, LANDSCAPE, PORTRAIT_INVERTED, LANDSCAPE_INVERTED
 */
#define SCREEN_ORIENTATION LANDSCAPE

/**
 * screen width in pixels
 */
#define SCREEN_WIDTH 272 // px

/**
 * screen height in pixels
 */
#define SCREEN_HEIGHT 480 // px
#endif