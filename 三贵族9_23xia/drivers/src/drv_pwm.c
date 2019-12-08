#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"

void PWM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	// 1\使能相应外设的时钟（GPIOC\TIM3\AFIO）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// 2\GPIO工作模式的配置
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);				// TIM3完全重映射
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// 3\定时器工作参数的配置
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1;		// 分频值
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 255;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;		// 时钟分割
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

	// 4\输出比较单元参数的配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;				// PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	// 输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;								// 初始的比较值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;		// 输出极性为低电平
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);			// 使能预装载功能，使用该功能能够实现3个通道的数据同步变更

	TIM_OC2Init(TIM3, &TIM_OCInitStruct);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_OC3Init(TIM3, &TIM_OCInitStruct);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);							// 使能自动重装载的预装载功能
	// 5\使能定时器
	TIM_Cmd(TIM3, ENABLE);
}

void PWM_SetRgb(uint8_t r, uint8_t g, uint8_t b)
{
	TIM_SetCompare3(TIM3, r);
	TIM_SetCompare2(TIM3, g);
	TIM_SetCompare1(TIM3, b);
}

