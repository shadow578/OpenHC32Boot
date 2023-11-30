#pragma once
#include "../sd/fatfs/ff.h"

/**
 * @brief prepare firmware update from the given file path.
 * @param filePath path to the firmware binary file.
 * @param updateFile file object opened for reading the firmware binary file.
 * @return true if the firmware update is ready to be applied
 */
bool prepare_firmware_update(const char *filePath, FIL &updateFile);

/**
 * @brief attempt to update the firmware from the given file.
 * @param updateFile file object to read the firmware binary file from.
 * @param baseAddress base address to write the firmware binary file to.
 * @return true if the firmware update was successful
 */
bool apply_firmware_update(FIL &updateFile, uint32_t baseAddress);

/**
 * @brief end the firmware update process.
 * @param updateFile file object to close.
 */
void end_firmware_update(FIL &updateFile);
