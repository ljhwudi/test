#include "stm32f10x.h"




void Leds_BeepCfg(uint16_t GPIO_Pin,FunctionalState NewState)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
		GPIO_InitTypeDef GPIO_InitStructure; 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
		GPIO_SetBits(GPIOC, GPIO_Pin);
		
		if(ENABLE==NewState)
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin);
		}
		if(DISABLE==NewState)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin);	
		}
}


