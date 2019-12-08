#include <string.h>
#include "gizwits_product.h"
#include "common.h"
#include "drv_adc.h"
#include "stm32f10x.h"
#include "misc.h"
#include "drv_systick.h"
#include "drv_uart.h"
#include "drv_tim.h"
#include "drv_led.h"
#include "drv_K1.h"
#include "drv_fan.h"
#include "drv_window.h"
#include "drv_water.h"
#include "drv_test_adx.h"
#include "drv_dht11.h"

DHT11_Data_TypeDef dht11_data;
#define PROTOCOL_DEBUG
extern u8 USART2_RX_BUF[300]; 	
 

int main(int argc, char *argv[])
{
	char data[300];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SYSTICK_Config(72);
	UART1_Config();  //串口一负责发送信息
	UART2_Config();   //串口二负责机智云
//	TIM2_Config();
//	TIM6_Config();

	FAN_Init();  //风扇
	WINDOW_Init(); //步进电机
	ADC_Config();

	
	SYSTICK_DelayMs(10000);
//		sprintf(data,"{ \"flag\": 1, \"acct\": \"33\", \"pswd\": \"1\", \"reflag\": 4, \"cmd\": 1108497911, \"temp\": \"45\", \"hum\": \"456\", \"smoke\": \"\", \"ch2o\": \"\", \"ch4\": \"\", \"pm2p5\": \"\" }");
//	usart2_print("{ \"flag\": 1, \"acct\": \"33\", \"pswd\": \"1\", \"reflag\": 4, \"cmd\": 1108497911, \"temp\": \"\", \"hum\": \"\", \"smoke\": \"\", \"ch2o\": \"\", \"ch4\": \"\", \"pm2p5\": \"\" }");
//		usart2_print(data);
		int  tmp;
		float temp,hum;
		u16 smoke,co,ch4,pm2p5;
	while(1)
	{
	//	printf("pc5:%d\npc2:%d",);
		sscanf(USART2_RX_BUF,"{ \"cmd\": %d, \"fl",&tmp);
//		printf("%s\n",USART2_RX_BUF);
		printf("%d\n",tmp);
		if(tmp&(1<<15))
		{
			if(tmp&(1<<14))
			{
				//开灯
				led_on();
			}
			else
			{
					//关灯
				led_off();
			}
		}
		
		if(tmp&(1<<13))
		{
			if(tmp&(1<<12))
			{
				//开插座
				
			}
			else
			{
					//关插座
				
			}
		}
			
		if(tmp&(1<<11))
		{
			if(tmp&(1<<10))
			{
				//开风扇
				FAN_Control(ENABLE);
			}
			else
			{
					//关风扇
				FAN_Control(DISABLE);
			}
		}
			
		if(tmp&(1<<9))
		{
			WINDOW_Open(tmp&0xff);
		}	
		
		Read_DHT11(&dht11_data);
		
		temp = (float)(dht11_data.temp_int)+dht11_data.temp_deci/10.0;
		hum = (float)(dht11_data.humi_int)+dht11_data.humi_deci/10.0;
		smoke = ADC_GetsmokeVal();
		co = ADC_GetcoVal();
		ch4 = 3333;
		pm2p5 = 4000;
		
		sprintf(data,"{ \"flag\": 10, \"acct\": \"33\", \"pswd\": \"1\", \"reflag\": 4, \"cmd\": 1108497911, \"temp\": \"%.1f\", \"hum\": \"%.1f\", \"smoke\": \"%d\", \"co\": \"%d\", \"ch4\": \"%d\", \"pm2p5\": \"%d\" }",temp,hum,smoke,co,ch4,pm2p5);
		usart2_print(data);
		
		SYSTICK_DelayMs(1000);
	}
	
}

