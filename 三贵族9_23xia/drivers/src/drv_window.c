#include "drv_window.h"
#include "drv_systick.h"

#define ORANGE_PERIPH_PORT		RCC_APB2Periph_GPIOD
#define YELLOW_PERIPH_PORT		RCC_APB2Periph_GPIOC
#define PINK_PERIPH_PORT		RCC_APB2Periph_GPIOC
#define BLUE_PERIPH_PORT		RCC_APB2Periph_GPIOC

#define ORANGE_PORT				GPIOD
#define YELLOW_PORT				GPIOC
#define PINK_PORT				GPIOC
#define BLUE_PORT				GPIOC

#define ORANGE_PIN				GPIO_Pin_2
#define YELLOW_PIN				GPIO_Pin_12
#define PINK_PIN				GPIO_Pin_11
#define BLUE_PIN				GPIO_Pin_10

#define PIN_HIGH_FUNC			GPIO_SetBits
#define PIN_LOW_FUNC			GPIO_ResetBits

//static u8 direflag=0;
static u8 win_open_arr[8]={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};   //逆时钟旋转相序表
//static u8 win_close_arr[8]={0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};    //正时钟旋转相序表
static u16 flag = 0;
static u16 targetflag = 0;

void WINDOW_Init()
{

	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(ORANGE_PERIPH_PORT | YELLOW_PERIPH_PORT | PINK_PERIPH_PORT | BLUE_PERIPH_PORT, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = ORANGE_PIN ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

	GPIO_Init(ORANGE_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = YELLOW_PIN;

	GPIO_Init(YELLOW_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = PINK_PIN;

	GPIO_Init(PINK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = BLUE_PIN;

	GPIO_Init(BLUE_PORT, &GPIO_InitStruct);
	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	// 1MS进一次中断
	TIM_TimeBaseInitStruct.TIM_Period = 10-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);			// 使能更新中断
	

	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);

	TIM_Cmd(TIM7, DISABLE);
}

void WINDOW_PinControl(uint16_t GPIO_Pin,FunctionalState NewState)
{
	if(ENABLE == NewState)
	{
		if(ORANGE_PIN == GPIO_Pin)
		{
			PIN_HIGH_FUNC(ORANGE_PORT,GPIO_Pin);	
		}
		else if(YELLOW_PIN == GPIO_Pin)
		{
			PIN_HIGH_FUNC(YELLOW_PORT,GPIO_Pin);
		}
		else if(PINK_PIN == GPIO_Pin)
		{
			PIN_HIGH_FUNC(PINK_PORT,GPIO_Pin);
		}
		else if(BLUE_PIN == GPIO_Pin)
		{
			PIN_HIGH_FUNC(BLUE_PORT,GPIO_Pin);
		}
	}
	else
	{
		if(ORANGE_PIN == GPIO_Pin)
		{
			PIN_LOW_FUNC(ORANGE_PORT,GPIO_Pin);
		}
		else if(YELLOW_PIN == GPIO_Pin)
		{
			PIN_LOW_FUNC(YELLOW_PORT,GPIO_Pin);
		}
		else if(PINK_PIN == GPIO_Pin)
		{
			PIN_LOW_FUNC(PINK_PORT,GPIO_Pin);
		}
		else if(BLUE_PIN == GPIO_Pin)
		{
			PIN_LOW_FUNC(BLUE_PORT,GPIO_Pin);
		}
	}
		
}



void PIN_Cfg(u8 t)
{
	if(t&0x01)
		WINDOW_PinControl(BLUE_PIN, ENABLE);
	else
		WINDOW_PinControl(BLUE_PIN, DISABLE);
	if(t&0x02)
		WINDOW_PinControl(PINK_PIN, ENABLE);
	else
		WINDOW_PinControl(PINK_PIN, DISABLE);
	if(t&0x04)
		WINDOW_PinControl(YELLOW_PIN, ENABLE);
	else
		WINDOW_PinControl(YELLOW_PIN, DISABLE);
	if(t&0x08)
		WINDOW_PinControl(ORANGE_PIN, ENABLE);
	else
		WINDOW_PinControl(ORANGE_PIN, DISABLE);
}

void WINDOW_Open(u8 angle)
{

	targetflag = 16*angle;
//	TIM_Cmd(TIM7, DISABLE);
	TIM_Cmd(TIM7, ENABLE);
	
}


void TIM7_IRQHandler()
{
   if(TIM_GetITStatus(TIM7, TIM_IT_Update))
   	{
   		PIN_Cfg(win_open_arr[flag%8]);
	   
	   if(targetflag > flag)
	   {
			flag++;
	   }
	   else if(targetflag < flag)
	   {
			flag--;
	   }
		 else if(targetflag == flag)
	   {
			TIM_Cmd(TIM7, DISABLE);
	   }
	   TIM_ClearITPendingBit(TIM7, TIM_IT_Update); 
   	}
}

