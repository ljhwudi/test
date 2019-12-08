#include "drv_water.h"

#define WATER_PERIPH_CLK_FUNC			RCC_APB2PeriphClockCmd
#define WATER_PORT						GPIOC

#define WATER_PERIPH_PORT				RCC_APB2Periph_GPIOC
#define WATER_PIN						GPIO_Pin_13

#define WATER_ON_FUNC()					GPIO_ResetBits(WATER_PORT,WATER_PIN)
#define WATER_OFF_FUNC()				GPIO_SetBits(WATER_PORT,WATER_PIN)


void WATER_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	WATER_PERIPH_CLK_FUNC(WATER_PERIPH_PORT,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = WATER_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

	GPIO_Init(WATER_PORT, &GPIO_InitStruct);
	WATER_OFF_FUNC();
}

void WATER_Control(FunctionalState NewState)
{
	if(ENABLE == NewState)
		WATER_ON_FUNC();
	else
		WATER_OFF_FUNC();
}

