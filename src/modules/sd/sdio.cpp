#include "sdio.h"
#include "../log.h"
#include <sd_card.h>

#include "../assert.h"

#include <source/diskio.h>

#if PF_USE_WRITE
  #error "FatFS write is not supported!"
#endif

constexpr size_t SD_BLOCK_SIZE = 512;
constexpr DWORD LSECTOR_INVALID = 0xFFFFFFFF;

#define SDIO_UNIT CONCAT(M4_SDIOC, SDIO_PERIPHERAL);

/**
 * @brief get SDIO bus width
 * @param width number of pins used for data
 * @return bus width
 */
constexpr en_sdioc_bus_width_t get_sdioc_bus_width(const int width)
{
  switch (width)
  {
  default:
  case 1:
    return SdiocBusWidth1Bit;
  case 4:
    return SdiocBusWidth4Bit;
  case 8:
    return SdiocBusWidth8Bit;
  }
}

/**
 * @brief SD card handle 
 */
stc_sd_handle_t *handle = nullptr;

/**
 * @brief initialize drive
 * @return status. byte with one or more of [STA_NOINIT, STA_NODISK] set
 */
extern "C" DSTATUS disk_initialize(void)
{
  // set CLK pin to medium drive strength
  // stc_port_init_t clockPinInit = {
  //  .enPinDrv = Pin_Drv_M,
  // };
  // PORT_Init(sdio::pins.clk.port, sdio::pins.clk.pin, &clockPinInit);
  
  // configure SDIO pins:
  // 1-bit bus width
  PORT_SetFunc(sdio::pins.dat[0].port, sdio::pins.dat[0].pin, Func_Sdio, Disable);

  // 4 and 8-bit bus width
  if (sdio::bus_width > 1)
  {
    PORT_SetFunc(sdio::pins.dat[1].port, sdio::pins.dat[1].pin, Func_Sdio, Disable);
    PORT_SetFunc(sdio::pins.dat[2].port, sdio::pins.dat[2].pin, Func_Sdio, Disable);
    PORT_SetFunc(sdio::pins.dat[3].port, sdio::pins.dat[3].pin, Func_Sdio, Disable);
  }

  // 8-bit bus width
  if (sdio::bus_width > 4)
  {
    PORT_SetFunc(sdio::pins.dat[4].port, sdio::pins.dat[4].pin, Func_Sdio, Disable);
    PORT_SetFunc(sdio::pins.dat[5].port, sdio::pins.dat[5].pin, Func_Sdio, Disable);
    PORT_SetFunc(sdio::pins.dat[6].port, sdio::pins.dat[6].pin, Func_Sdio, Disable);
    PORT_SetFunc(sdio::pins.dat[7].port, sdio::pins.dat[7].pin, Func_Sdio, Disable);
  }

  // CLK, CMD and DET
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
  cardConf->enBusWidth = get_sdioc_bus_width(sdio::bus_width);
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
    return STA_NODISK;
  }

  // everything is OK
  return 0;
}

/**
 * @brief read partial sector
 * @param buff pointer to the data buffer to store read data
 * @param sector sector number to read
 * @param offset offset in the sector
 * @param count byte count
 * @return status. one of [RES_OK, RES_ERROR, RES_NOTRDY, RES_PARERR]
 */
extern "C" DRESULT disk_readp(BYTE *buff, DWORD sector, UINT offset, UINT count)
{
  static BYTE block_buffer[SD_BLOCK_SIZE];
  static DWORD last_sector = LSECTOR_INVALID;

  // if either offset or count is larger than the block buffer, we have a problem.
  // since we first read into the block buffer, we can't read more than that.
  ASSERT(offset < SD_BLOCK_SIZE, "offset >= SD_BLOCK_SIZE");
  ASSERT(count <= SD_BLOCK_SIZE, "count > SD_BLOCK_SIZE");

  // check if handle is initialized
  if (handle == nullptr)
  {
    return RES_NOTRDY;
  }

  // read 1 full block / sector if different from last read
  // Petit FatFS wants to read partial sectors, but the DDL only allows reading full ones.
  // So we read the full sector and then copy "chunks" to FatFS's buffer.
  // Since FatFS may request the same (partial) sector multiple times, we cache the full sector buffer and 
  // use the cache if the sector matches.
  logging::debug("read sector ");
  logging::debug(sector, 10);
  if (sector != last_sector)
  {
    logging::debug(" from DISK\n");

    en_result_t rc = SDCARD_ReadBlocks(handle, sector, 1, block_buffer, sdio::read_timeout);
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
  }
  else
  {
    // read from cache
    logging::debug(" from CACHE\n");
  }

  // copy bytes from block SDIO buffer to FatFS buffer
  memcpy(buff, block_buffer + offset, count);
  return RES_OK;
}
