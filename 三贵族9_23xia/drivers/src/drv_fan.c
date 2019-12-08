#include "drv_fan.h"
#include "stdio.h"

#define FAN_PERIPH_CLK_FUNC			RCC_APB2PeriphClockCmd
#define FAN_PORT					GPIOB

#define FAN_PERIPH_PORT				RCC_APB2Periph_GPIOB
#define FAN_PIN						GPIO_Pin_5

#define FAN_ON_FUNC()				GPIO_ResetBits(FAN_PORT,FAN_PIN)
#define FAN_OFF_FUNC()				GPIO_SetBits(FAN_PORT,FAN_PIN)


void FAN_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	FAN_PERIPH_CLK_FUNC(FAN_PERIPH_PORT,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = FAN_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

	GPIO_Init(FAN_PORT, &GPIO_InitStruct);

	FAN_OFF_FUNC();
}

void FAN_Control(FunctionalState NewState)
{
	
	if(ENABLE == NewState)
		FAN_ON_FUNC();
	else
		FAN_OFF_FUNC();
}
