#include "drv_mq2.h"
#include "drv_adc.h"
#include "drv_systick.h"
#include <stdio.h>
#include "drv_fan.h"
#include "drv_window.h"
#include "drv_water.h"
#include "drv_tim.h"

static u8 yanwuflag;

void Mq2_Control(void)
{
		
		if(ADC_GetsmokeVal() >= 1000)
		{
			if(yanwuflag == 0)
			{		
				printf("smoke warning!\n");
					LED_R_flash();
				GPIO_ResetBits(GPIOC, GPIO_Pin_9);
				
				WATER_Control(ENABLE);
				FAN_Control(ENABLE);
				WINDOW_Open(11);
					yanwuflag = 1;
				}
				
			}
		else
		{
			if(yanwuflag == 1)
			{
				LED_R_noflash();
				FAN_Control(DISABLE);
				GPIO_SetBits(GPIOC, GPIO_Pin_9);
				WATER_Control(DISABLE);
					yanwuflag = 0;
			}
		}

}
