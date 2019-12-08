#ifndef __DRV_DMA_H__
#define __DRV_DMA_H__

#include "stm32f10x.h"

void DMA_Config(uint32_t *src, uint32_t *dst, uint16_t size);
uint16_t DMA_TransmitCompare(uint32_t *src, uint32_t *dst, uint16_t size);


#endif // __DRV_DMA_H__

