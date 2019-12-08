#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"

void DMA_Config(uint32_t *src, uint32_t *dst, uint16_t size)
{
	DMA_InitTypeDef DMA_InitStruct;
	// 开启DMA外设的时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)src;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)dst;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;				// 外设作为数据的来源（该参量的取值，取决于上述两个地址的赋值）
	DMA_InitStruct.DMA_BufferSize = size;						// 传输数目（传输 数目不一定等于传输字节数，与传输宽度有关）
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//外设基址的增量使能
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;		// 存储器基址的增量使能
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	// 外设数据宽度
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;// 存储器数据宽度
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;					// 一次传输
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;		// 优先权
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;					// 存储器到存储器传输
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
}

uint16_t DMA_TransmitCompare(uint32_t *src, uint32_t *dst, uint16_t size)
{
	uint16_t i;

	for (i=0; i<size; i++)
	{
		if (src[i] != dst[i])
			break;
	}

	return i;
}
