#include "update.h"
#include <Arduino.h>
#include "../ui/ui.h"
#include "../config.h"
#include <stdio.h>
#include <hc32_ddl.h>
#include <core_debug.h>

/**
 * @brief flash erase sector size
 * @note on HC32F460 this is 8Kb
 */
#define FLASH_ERASE_SECTOR_SIZE 8192

/**
 * @brief size of the file buffer
 * @note must be aligned to 32-bit words
 */
#define FILE_BUFFER_SIZE 512

/**
 * @brief buffer used for reading from the firmware update file
 */
BYTE file_buffer[FILE_BUFFER_SIZE];

#if (APP_BASE_ADDRESS % FLASH_ERASE_SECTOR_SIZE) != 0
#error "APP_BASE_ADDRESS must be aligned to flash erase sector boundary"
#endif

/**
 * @brief highest address of the writable flash area
 * @note for the 512K flash version
 * @note
 * 0x0007FFE0 - 0x0007FFFF is reserved and cannot be used.
 * refer to the user manual, section 7.3 "Embedded FLASH"
 */
#define MAX_FLASH_ADDRESS (0x0007FFE0u - 1)

/**
 * @brief lowest address of the writable flash area
 * @note set in update_from_file before any flash operation
 */
uint32_t MIN_FLASH_ADDRESS = MAX_FLASH_ADDRESS;

/**
 * @brief assert that the given address is inside the flash area usable for the application
 * @note excludes the bootloader area
 * @note accessing flash above 0x0003FFFFu on the 256K flash model will just do nothing, so just check for the 512K flash model here
 */
#define ASSERT_INSIDE_FLASH_AREA(address, ...) CORE_ASSERT(address >= MIN_FLASH_ADDRESS && address <= MAX_FLASH_ADDRESS, "address outside flash area", ##__VA_ARGS__)

/**
 * FatFS file system object
 */
FATFS fs;

/**
 * @brief erase the flash sectors
 * @param startAddress the address to start erasing at
 * @param endAddress the address to stop erasing at
 * @return true if successful, false otherwise
 *
 * @note erases up to the nearest sector boundaries, so more than requested may be erased
 * @note assumes EFM is already unlocked and enabled
 */
inline bool erase_flash(uint32_t startAddress, uint32_t endAddress)
{
    ASSERT_INSIDE_FLASH_AREA(startAddress, return false);
    ASSERT_INSIDE_FLASH_AREA(endAddress, return false);

    // get sector addresses
    uint32_t startSectorNo = startAddress / FLASH_ERASE_SECTOR_SIZE; // inclusive
    uint32_t endSectorNo = endAddress / FLASH_ERASE_SECTOR_SIZE;     // inclusive

    // calculate total sector count for progress calculation
    uint32_t totalSectorCount = (endSectorNo - startSectorNo) + 1;

    // erase sectors
    for (uint32_t sectorNo = startSectorNo; sectorNo <= endSectorNo; sectorNo++)
    {
        uint32_t sectorAddress = sectorNo * FLASH_ERASE_SECTOR_SIZE;
        printf("erase sector %ld @0x%08lx\n", sectorNo, sectorAddress);

        // erase sector
#if (UPDATE_DRY_RUN == 1)
        printf("EFM_SectorErase(0x%08lx)\n", sectorAddress);
        en_result_t rc = Ok;
#else
        en_result_t rc = EFM_SectorErase(sectorAddress);
#endif
        if (rc != Ok)
        {
            printf("EFM_SectorErase() rc=%d\n", rc);
            return false;
        }

        // update UI
        UI::show_progress(((sectorNo - startSectorNo) * 100) / totalSectorCount);
    }

    // nothing failed, return success
    return true;
}

/**
 * @brief program the flash with the given data from the given file, and verify the data afterwards
 * @param address the address to start programming at
 * @param updateFile the file to read the data from. expected to be at the start of the file
 * @param fileSize the size of the file
 * @return true if successful, false otherwise
 *
 * @note assumes EFM is already unlocked and enabled
 * @note assumes the flash has already been erased
 */
inline bool program_flash(uint32_t address, FIL *updateFile, uint32_t fileSize)
{
    DWORD totalBytesWritten = 0;
    while (true)
    {
        // read next block from file
        UINT bytesRead = 0;
        FRESULT res = f_read(updateFile, file_buffer, FILE_BUFFER_SIZE, &bytesRead);
        if (res != FR_OK)
        {
            printf("f_read() failed: %d\n", res);
            return false;
        }

        // check for end of file
        if (bytesRead == 0)
        {
            // end of file, we're done
            return true;
        }

        // pad buffer with 0xff if not aligned to 32-bit word
        while ((bytesRead % 4) != 0)
        {
            file_buffer[bytesRead++] = 0xff;
        }

        // convert bytes to 32-bit words
        uint32_t *buffer32 = (uint32_t *)file_buffer;
        size_t buffer32Size = bytesRead / 4;

        // write block to flash using single program mode
        // note: sequence programming would require the algorithm to be executed from RAM
        printf("write 0x%08lx - 0x%08lx\n", address, address + bytesRead - 1);
        for (uint32_t i = 0, addr = address; i < buffer32Size; i++, addr += 4)
        {
            ASSERT_INSIDE_FLASH_AREA(addr, return false);
            uint32_t data = buffer32[i];

#if (UPDATE_DRY_RUN == 1)
            printf("EFM_SingleProgram(0x%08lx, 0x%08lx)\n", addr, data);
            en_result_t rc = Ok;
#else
            en_result_t rc = EFM_SingleProgram(addr, data);
#endif
            if (rc != Ok)
            {
                printf("EFM_SingleProgram() rc=%d\n", rc);
                return false;
            }
        }

        // verify block in flash
        printf("verify 0x%08lx - 0x%08lx\n", address, address + bytesRead - 1);
        if (memcmp(file_buffer, (void *)address, bytesRead) != 0)
        {
            printf("memcmp() failed\n");
            return false;
        }

        // increment address and total bytes written
        address += bytesRead;
        totalBytesWritten += bytesRead;

        // print progress
        UI::show_progress(totalBytesWritten * 100 / fileSize);
    }

    // should never get here
    return false;
}

bool prepare_firmware_update(const char *filePath, FIL &updateFile)
{
    // mount sd card
    FRESULT res = f_mount(&fs, "", /*mount now*/ 1);
    if (res != FR_OK)
    {
        // assume no sd card
        printf("f_mount() failed: %d\n", res);
        return false;
    }

    // try to open firmware update file
    res = f_open(&updateFile, filePath, FA_READ);
    if (res != FR_OK)
    {
        // assume file not found
        printf("f_open() failed: %d\n", res);
        return false;
    }

    // all ok if file is more than 0 bytes
    return f_size(&updateFile) > 0;
}

bool apply_firmware_update(FIL &updateFile, uint32_t baseAddress)
{
    MIN_FLASH_ADDRESS = baseAddress;

    // get total size of update file
    const DWORD updateSize = f_size(&updateFile);

    // ensue update file fits inside the available flash area for the application
    if (updateSize > (MAX_FLASH_ADDRESS - MIN_FLASH_ADDRESS))
    {
        printf("update file too large\n");
        return false;
    }

    // unlock and enable flash
    EFM_Unlock();
    EFM_FlashCmd(Enable);

    // wait until ready flag is set
    while (Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        ;
    }

    // disable interrupts during flash programming
    // noInterrupts();

    // erase flash
    printf("erase flash\n");
    if (!erase_flash(baseAddress, baseAddress + updateSize))
    {
        printf("erase failed\n");
        return false;
    }

    // program & verify flash
    printf("program flash\n");
    if (!program_flash(baseAddress, &updateFile, updateSize))
    {
        printf("program failed\n");
        return false;
    }

    // re-enable interrupts
    // interrupts();

    // re-lock flash
    EFM_Lock();

    // nothing failed, return success
    return true;
}

void end_firmware_update(FIL &updateFile)
{
    // close update file
    FRESULT res = f_close(&updateFile);
    if (res != FR_OK)
    {
        printf("f_close() failed: %d\n", res);
    }

#if (DELETE_FIRMEWARE_UPDATE_FILE == 1)
    // delete update file
    res = f_unlink(FIRMWARE_UPDATE_FILE_PATH);
    if (res != FR_OK)
    {
        printf("f_unlink() failed: %d\n", res);
    }
#endif

    // unmount sd card
    res = f_unmount("");
    if (res != FR_OK)
    {
        printf("f_unmount() failed: %d\n", res);
    }
}
