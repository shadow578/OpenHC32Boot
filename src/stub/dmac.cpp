#include "dmac.h"

void DMA_Cmd(M4_DMA_TypeDef* pstcDmaReg, en_functional_state_t enNewState)
{
  __BKPT(0);
}

void DMA_InitChannel(M4_DMA_TypeDef* pstcDmaReg, uint8_t u8Ch, const stc_dma_config_t* pstcDmaCfg) 
{
  __BKPT(0);
}

en_result_t DMA_ChannelCmd(M4_DMA_TypeDef* pstcDmaReg, uint8_t u8Ch, en_functional_state_t enNewState) 
{
  __BKPT(0);
  return Error;
}

en_result_t DMA_ClearIrqFlag(M4_DMA_TypeDef* pstcDmaReg, uint8_t u8Ch, en_dma_irq_sel_t enIrqSel) 
{
  __BKPT(0);
  return Error;
}

void DMA_SetTriggerSrc(const M4_DMA_TypeDef* pstcDmaReg, uint8_t u8Ch, en_event_src_t enSrc) 
{
  __BKPT(0);
}
