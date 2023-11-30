#include <Arduino.h>
#include <hc32_ddl.h>
#include "../config.h"
#include <drivers/gpio/gpio.h>
#include <sd_card.h>

#include "fatfs/ff.h"
#include "fatfs/diskio.h"

#define READ_TIMEOUT 500  // ms
#define WRITE_TIMEOUT 500 // ms

/**
 * @brief sd card handle
 */
stc_sd_handle_t *handle = nullptr;

/**
 * @brief initialize drive
 * @param pdrv drive number to identify the drive
 * @return status. byte with one or more of [STA_NOINIT, STA_NODISK, STA_PROTECT] set
 */
extern "C" DSTATUS disk_initialize(BYTE pdrv)
{
    // set CLK pin to medium drive strength
    // stc_port_init_t clockPinInit = {
    //    .enPinDrv = Pin_Drv_M,
    // };
    // GPIO_Init(SDIO_CLK_PIN, &clockPinInit);

    // configure SDIO pins
    GPIO_SetFunc(SDIO_D0_PIN, Func_Sdio);
    GPIO_SetFunc(SDIO_D1_PIN, Func_Sdio);
    GPIO_SetFunc(SDIO_D2_PIN, Func_Sdio);
    GPIO_SetFunc(SDIO_D3_PIN, Func_Sdio);
    GPIO_SetFunc(SDIO_CLK_PIN, Func_Sdio);
    GPIO_SetFunc(SDIO_CMD_PIN, Func_Sdio);
    GPIO_SetFunc(SDIO_DET_PIN, Func_Sdio);

    // close handle if it exists
    if (handle != nullptr)
    {
        delete handle;
        // handle = nullptr;
    }

    // create handle in polling mode
    handle = new stc_sd_handle_t;
    handle->SDIOCx = M4_SDIOC1;
    handle->enDevMode = SdCardPollingMode;

    // create card configuration
    // this should be a fairly safe configuration for most cards
    stc_sdcard_init_t cardConf = {
        .enBusWidth = SdiocBusWidth4Bit,
        .enClkFreq = SdiocClk400K,
        .enSpeedMode = SdiocNormalSpeedMode,
        .pstcInitCfg = NULL,
    };

    // initialize sd card
    en_result_t rc = SDCARD_Init(handle, &cardConf);
    if (rc == Ok)
    {
        printf("sd init success\n");
    }
    else
    {
        printf("sd init fail (rc=%u)\n", rc);
        return STA_NOINIT; // error initializing
    }

    // read card CSD
    rc = SDCARD_GetCardCSD(handle);
    if (rc == Ok)
    {
        printf("sd get csd success\n");
    }
    else
    {
        printf("sd get csd fail (rc=%u)\n", rc);
        return STA_NOINIT; // error initializing
    }

    // all ok
    return 0;
}

/**
 * @brief get drive status
 * @param pdrv drive number to identify the drive
 * @return status. byte with one or more of [STA_NOINIT, STA_NODISK, STA_PROTECT] set
 */
extern "C" DSTATUS disk_status(BYTE pdrv)
{
    if (handle == nullptr)
    {
        // not initialized
        return STA_NOINIT;
    }

    if (handle->stcCardStatus.CARD_IS_LOCKED)
    {
        // card is locked
        return STA_PROTECT;
    }

    if (handle->stcCardStatus.READY_FOR_DATA)
    {
        // card is ready
        return 0;
    }

    // card is not ready
    return STA_NOINIT;
}

/**
 * @brief read sectors
 * @param pdrv drive number to identify the drive
 * @param buff pointer to the data buffer to store read data
 * @param sector start sector number to read
 * @param count number of sectors to read
 * @return status. one of [RES_OK, RES_ERROR, RES_WRPRT, RES_NOTRDY, RES_PARERR]
 */
extern "C" DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
    // check if handle is initialized
    if (handle == nullptr)
    {
        return RES_NOTRDY;
    }

    // read sectors
    en_result_t rc = SDCARD_ReadBlocks(handle, sector, count, buff, READ_TIMEOUT);
    if (rc == Ok)
    {
        return RES_OK;
    }
    else
    {
        printf("sd read fail (rc=%u; err=%lu; @%lu)\n", rc, handle->u32ErrorCode, sector);
        return RES_ERROR;
    }
}

#if FF_FS_READONLY == 0

/**
 * @brief write sectors
 * @param pdrv drive number to identify the drive
 * @param buff data to write
 * @param sector start sector number to write
 * @param count number of sectors to write
 * @return status. one of [RES_OK, RES_ERROR, RES_WRPRT, RES_NOTRDY, RES_PARERR]
 */
extern "C" DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{
    // check if handle is initialized
    if (handle == nullptr)
    {
        return RES_NOTRDY;
    }

    // write sectors
    en_result_t rc = SDCARD_WriteBlocks(handle, sector, count, (uint8_t *)buff, WRITE_TIMEOUT);
    if (rc == Ok)
    {
        return RES_OK;
    }
    else
    {
        printf("sd write fail (rc=%u; err=%lu; @%lu)\n", rc, handle->u32ErrorCode, sector);
        return RES_ERROR;
    }
}

#endif // FF_FS_READONLY

/**
 * @brief ioctl function
 * @param pdrv drive number to identify the drive
 * @param cmd control command code
 * @param buff pointer to the control data. read/write depends on the command code
 * @return status. one of [RES_OK, RES_ERROR, RES_WRPRT, RES_NOTRDY, RES_PARERR]
 */
extern "C" DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    // ensure handle is initialized
    if (handle == nullptr)
    {
        return RES_NOTRDY;
    }

    // handle command
    switch (cmd)
    {
    case CTRL_SYNC:
        // wait for card to be ready again
        while (!handle->stcCardStatus.READY_FOR_DATA)
        {
            // wait
        }
        return RES_OK;
    case GET_SECTOR_COUNT:
        // get sector count
        // in DDL, they are called "blocks"
        *(LBA_t *)buff = handle->stcSdCardInfo.u32LogBlockNbr;
        return RES_OK;
    case GET_SECTOR_SIZE:
        // get sector size
        *(WORD *)buff = handle->stcSdCardInfo.u32LogBlockSize;
        return RES_OK;
    // case GET_BLOCK_SIZE:
    //     // get block size
    //     *(DWORD *)buff = 1;
    //     return RES_OK;
    // case CTRL_TRIM:
    //     // trim
    //     return RES_OK;
    default:
        // unknown command
        printf("unknown ioctl command %u\n", cmd);
        return RES_PARERR;
    }
}
