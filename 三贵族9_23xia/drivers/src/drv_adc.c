#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "math.h"
#include "drv_adc.h"

#define NRB_OF_CH					4
#define CONVER_NUM					50

volatile uint16_t adcConverVal[CONVER_NUM][NRB_OF_CH] = {0};

static uint16_t adcGetChVal(uint8_t ch)
{
	uint32_t sum = 0;
	uint16_t i;

	for (i=0; i<CONVER_NUM; i++)
	{
		sum += adcConverVal[i][ch];
	}

	return (uint16_t)(sum/CONVER_NUM);
}

void ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;

	// 1\开启相应外设的时钟（GPIOC、ADC1、AFIO、DMA1）
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// ADC的时钟为6分频  （72MHz/6=12）
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	// 2\GPIO引脚的工作模式的配置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// 3\ADC工作参数的配置
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;				// 配置ADC1为独立工作模式
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;					// 使能扫描模式
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;				// 使能连续转换
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// 不配置外部触发，最后由软件来触发
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;			// 数据对齐方式为右对齐
	ADC_InitStruct.ADC_NbrOfChannel = NRB_OF_CH;				// 当前为2路
	ADC_Init(ADC1, &ADC_InitStruct);

	// 4\相应的ADC工作通道的参数配置
	// 参数1：具体的ADC器件
	// 参数2：表示当前的具体转换通道
	// 参数3：表示该通道第几个进行转换，当前为第一个
	// 参数4：采样周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 4, ADC_SampleTime_239Cycles5);

	// 5\ADC的校准
	ADC_Cmd(ADC1, ENABLE);						// ADC1使能
	ADC_ResetCalibration(ADC1);					// 重置校准
	while (ADC_GetResetCalibrationStatus(ADC1));// 等待重置校准完成
	ADC_StartCalibration(ADC1);					// 开启校准
	while (ADC_GetCalibrationStatus(ADC1));		// 等待校准完成

	// 6\配置DMA
	ADC_DMACmd(ADC1, ENABLE);					// 使能ADC1的DMA请求
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)adcConverVal;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;					// 外设作为数据的来源
	DMA_InitStruct.DMA_BufferSize = CONVER_NUM * NRB_OF_CH;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	// 外设基址的增量失能（所有的数据都来源与DR寄存器）
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;			// 存储器地址增量使能（不同的采集批次放置在不同存储单元）
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// 外设的数据宽度，设置成半字
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		// 储存器的数据宽度，设置成半字
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;							// 模式选择循环
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;								// 存储器到存储器失能
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	DMA_Cmd(DMA1_Channel1, ENABLE);											// 使能DMA

	// 7\触发转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);							// 软件触发ADC1转换
}

uint16_t ADC_GetRgVal(void)
{
	return adcGetChVal(0);
}

uint16_t ADC_GetAdkeyVal(void)
{
	return adcGetChVal(1);
}

//烟雾传感器
uint16_t ADC_GetsmokeVal(void)
{
	return adcGetChVal(2);
}

//乙醇传感器
uint16_t ADC_GetcoVal(void)
{
	return adcGetChVal(3);
}


int  whichadkeypressed(void)
{
	if(ADC_GetAdkeyVal()<(AD_K1NUM+AD_K2NUM)/2)
		return AD_K1;
	if((ADC_GetAdkeyVal()<(AD_K3NUM+AD_K2NUM)/2)&&(ADC_GetAdkeyVal()>(AD_K1NUM+AD_K2NUM)/2))
		return AD_K2;
	if((ADC_GetAdkeyVal()<(AD_K4NUM+AD_K3NUM)/2)&&(ADC_GetAdkeyVal()>(AD_K3NUM+AD_K2NUM)/2))
		return AD_K3;
	if((ADC_GetAdkeyVal()<(AD_K5NUM+AD_K4NUM)/2)&&(ADC_GetAdkeyVal()>(AD_K4NUM+AD_K3NUM)/2))
		return AD_K4;
	if((ADC_GetAdkeyVal()<4096/2)&&(ADC_GetAdkeyVal()>(AD_K5NUM+AD_K4NUM)/2))
		return AD_K5;
	if((ADC_GetAdkeyVal()>4096/2))
		return NO_AD_KEY;
	return NO_AD_KEY;
}




