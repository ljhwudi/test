#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "drv_tim.h"
#include "drv_ledsbeep.h"
#include "drv_tim.h"
#include "drv_dht11.h"
#include "drv_adc.h"
#include "stdio.h"
static u8 ledflag;

void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// 1MS进一次中断
	TIM_TimeBaseInitStruct.TIM_Period = 3000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);			// 使能更新中断

	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM2, DISABLE);
}

void TIM3_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// 1MS进一次中断
	TIM_TimeBaseInitStruct.TIM_Period = 10-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);			// 使能更新中断

	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM3, ENABLE);
}

void LED_R_flash()
{
	TIM_Cmd(TIM2, ENABLE);
	//Leds_BeepCfg(LED_RED,DISABLE);
}

void LED_R_noflash()
{
	ledflag = 0;
	
	TIM_Cmd(TIM2, DISABLE);
	Leds_BeepCfg(LED_RED,DISABLE);
}

void TIM6_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	//定时器工作参数的配置
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInitStruct.TIM_Period = 10-1;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//使能TIM6的中断
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
	//使能定时器
	TIM_Cmd(TIM6, ENABLE);
}

void  TIM2_IRQHandler  ()
{
	
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)) 
{
	if(ledflag == 0)
	{
		Leds_BeepCfg( LED_RED,ENABLE);
		ledflag++;
	}
	else
	{
		Leds_BeepCfg( LED_RED,DISABLE);
		ledflag = 0;
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
} 

}
void TIM6_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
//		DHT11_Data_TypeDef Data;
//		
//			
//		Read_DHT11(&Data);
		printf("alcohol:%d\n", ADC_GetcoVal());
		printf("smoke:%d\n", ADC_GetsmokeVal());
//		printf("humidity:%d.%d\ntemperature:%d.%d\n",\
//		Data.humi_int,Data.humi_deci,Data.temp_int,Data.temp_deci);
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
	
}
