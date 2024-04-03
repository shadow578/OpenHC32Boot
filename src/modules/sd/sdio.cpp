#include "sdio.h"
#include "../log.h"
#include <sd_card.h>
#include <cstdio>

#include "fatfs/ff.h"
#include "fatfs/diskio.h"

#define SDIO_UNIT CONCAT(M4_SDIOC, SDIO_PERIPHERAL);

/**
 * @brief SD card handle 
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
  //  .enPinDrv = Pin_Drv_M,
  // };
  // PORT_Init(sdio::pins.clk.port, sdio::pins.clk.pin, &clockPinInit);
  
  // configure SDIO pins
  PORT_SetFunc(sdio::pins.d0.port, sdio::pins.d0.pin, Func_Sdio, Disable);
  PORT_SetFunc(sdio::pins.d1.port, sdio::pins.d1.pin, Func_Sdio, Disable);
  PORT_SetFunc(sdio::pins.d2.port, sdio::pins.d2.pin, Func_Sdio, Disable);
  PORT_SetFunc(sdio::pins.d3.port, sdio::pins.d3.pin, Func_Sdio, Disable);
  PORT_SetFunc(sdio::pins.clk.port, sdio::pins.clk.pin, Func_Sdio, Disable);
  PORT_SetFunc(sdio::pins.cmd.port, sdio::pins.cmd.pin, Func_Sdio, Disable);
  PORT_SetFunc(sdio::pins.det.port, sdio::pins.det.pin, Func_Sdio, Disable);

  // If a handle is already initialized, free it before creating a new one
  // otherwise, we will leak memory, which will eventually crash the system
  if (handle != nullptr) 
  {
    delete handle->pstcDmaInitCfg;
    delete handle->pstcCardInitCfg;
    delete handle;
    handle = nullptr;
  }

  // Create card configuration
  // This should be a fairly safe configuration for most cards
  stc_sdcard_init_t *cardConf = new stc_sdcard_init_t;
  cardConf->enBusWidth = SdiocBusWidth4Bit;
  cardConf->enClkFreq = SdiocClk400K;
  cardConf->enSpeedMode = SdiocNormalSpeedMode;
  cardConf->pstcInitCfg = nullptr;

  // Create handle in DMA mode
  handle = new stc_sd_handle_t;
  handle->SDIOCx = SDIO_UNIT;
  handle->enDevMode = SdCardPollingMode;
  //handle->pstcCardInitCfg = cardConf; // assigned in SDCARD_Init

  // Initialize sd card
  en_result_t rc = SDCARD_Init(handle, cardConf);
  if (rc != Ok) 
  {
    logging::debug("SDIO_Init() rc=");
    logging::debug(rc, 10);
    logging::debug("\n");
    return STA_NOINIT;
  }

  // read card CSD
  rc = SDCARD_GetCardCSD(handle);
  if (rc != Ok)
  {
    logging::debug("SDIO_GetCardCSD() rc=");
    logging::debug(rc, 10);
    logging::debug("\n");
    return STA_NOINIT;
  }

  // everything is OK
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
  en_result_t rc = SDCARD_ReadBlocks(handle, sector, count, buff, sdio::read_timeout);
  if (rc != Ok)
  {
    logging::debug("SDCARD_ReadBlocks() rc=");
    logging::debug(rc, 10);
    logging::debug(" err=");
    logging::debug(handle->u32ErrorCode, 10);
    logging::debug(" @ ");
    logging::debug(sector, 10);
    logging::debug("\n");
    return RES_ERROR;
  }

  return RES_OK;
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
  en_result_t rc = SDCARD_WriteBlocks(handle, sector, count, (uint8_t *)buff, sdio::write_timeout);
  if (rc == Ok)
  {
    return RES_OK;
  }
  else
  {
    logging::debug("SDCARD_WriteBlocks() rc=");
    logging::debug(rc, 10);
    logging::debug(" err=");
    logging::debug(handle->u32ErrorCode, 10);
    logging::debug(" @ ");
    logging::debug(sector, 10);
    logging::debug("\n");
    return RES_ERROR;
  }
}

#endif // FF_FS_READONLY == 0


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

  switch (cmd)
  {
  case CTRL_SYNC:
    // wait for card to be ready again
    while (!handle->stcCardStatus.READY_FOR_DATA) { /* nada */ }
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
  //case GET_BLOCK_SIZE:
  //  // get block size
  //  *(DWORD *)buff = 1;
  //  return RES_OK;
  //case CTRL_TRIM:
  //  // trim
  //  return RES_OK;
  default:
    // unknown command
    logging::debug("unknown ioctl command ");
    logging::debug(cmd, 10);
    logging::debug("\n");
    return RES_PARERR;
  }
}
