/**
 * minimal stub for the DMAC driver library (hc32f460_dmac.h), as 
 * required by the sd card middleware (sd_card.h).
 */

#ifndef STUB_DMAC_H
#define STUB_DMAC_H

#include <hc32_ddl.h>
#include <stdint.h>

typedef enum {
  DmaCh0, 
  DmaCh1, 
  DmaCh2, 
  DmaCh3
} en_dma_channel_t;

typedef enum {
  TrnErrIrq,
  TrnReqErrIrq,
  TrnCpltIrq,
  BlkTrnCpltIrq
} en_dma_irq_sel_t;

typedef enum {
  Dma8Bit,
  Dma16Bit,
  Dma32Bit
} en_dma_transfer_width_t;

typedef enum
{
    AddressFix,
    AddressIncrease,
    AddressDecrease
} en_dma_address_mode_t;

typedef struct
{
    en_dma_address_mode_t   enSrcInc;
    en_dma_address_mode_t   enDesInc;
    en_functional_state_t   enSrcRptEn;
    en_functional_state_t   enDesRptEn;
    en_functional_state_t   enSrcNseqEn;
    en_functional_state_t   enDesNseqEn;
    en_dma_transfer_width_t enTrnWidth;
    en_functional_state_t   enLlpEn;
    //en_dma_llp_mode_t       enLlpMd;
    en_functional_state_t   enIntEn;
}stc_dma_ch_cfg_t;

typedef struct
{
    uint16_t                u16BlockSize;
    uint16_t                u16TransferCnt;
    uint32_t                u32SrcAddr;
    uint32_t                u32DesAddr;
    uint16_t                u16SrcRptSize;
    uint16_t                u16DesRptSize;
    uint32_t                u32DmaLlp; 
    //stc_dma_nseq_cfg_t      stcSrcNseqCfg
    //stc_dma_nseq_cfg_t      stcDesNseqCfg
    stc_dma_ch_cfg_t        stcDmaChCfg;
} stc_dma_config_t;

#ifdef __cplusplus
extern "C" {
#endif

  void DMA_Cmd(M4_DMA_TypeDef* pstcDmaReg, en_functional_state_t enNewState);
  void DMA_InitChannel(M4_DMA_TypeDef* pstcDmaReg, uint8_t u8Ch, const stc_dma_config_t* pstcDmaCfg);
  en_result_t DMA_ChannelCmd(M4_DMA_TypeDef* pstcDmaReg, uint8_t u8Ch, en_functional_state_t enNewState);
  en_result_t DMA_ClearIrqFlag(M4_DMA_TypeDef* pstcDmaReg, uint8_t u8Ch, en_dma_irq_sel_t enIrqSel);
  void DMA_SetTriggerSrc(const M4_DMA_TypeDef* pstcDmaReg, uint8_t u8Ch, en_event_src_t enSrc);


#ifdef __cplusplus
}
#endif

#endif // STUB_DMAC_H
