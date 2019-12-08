#include "drv_test_adx.h"
#include "drv_tim.h"
#include "drv_fan.h"
#include "drv_window.h"
#include "drv_water.h"
#include "drv_ledsbeep.h"
#include "stdio.h"
static u8 yichunflag;
static u8 beepflag;
static u8 fanflag;
static u8 waterflag;
static u8 windowflag;
void test()
{
	//´¥·¢±¨¾¯µÆ
	
		if(whichadkeypressed()==AD_K1)
		{		
			SYSTICK_DelayMs(1);
			if(whichadkeypressed()==AD_K1)
			{
				LED_R_noflash();
				FAN_Control(DISABLE);
				Leds_BeepCfg(BEEP ,DISABLE);
				WATER_Control(DISABLE);

				beepflag=0;
				fanflag=0;
				waterflag=0;
				windowflag=0;
				while(whichadkeypressed()==AD_K1);
				SYSTICK_DelayUs(300);
	
			}
		}
	
		//´¥·¢¾¯±¨Éù£¨·äÃùÆ÷£©
		if(whichadkeypressed()==AD_K2)
		{
			SYSTICK_DelayMs(1);
			if(whichadkeypressed()==AD_K2)
			{
				if(beepflag == 0)
				{
					Leds_BeepCfg(BEEP ,ENABLE);
					beepflag = 1;
				}
				else
				{
					Leds_BeepCfg(BEEP ,DISABLE);
					beepflag = 0;
				}
					while(whichadkeypressed()==AD_K2);	
					SYSTICK_DelayUs(300);
			}
		}
	
		if(whichadkeypressed()==AD_K3)
		{
			SYSTICK_DelayMs(1);
			if(whichadkeypressed()==AD_K3)
			{
				if(fanflag == 0)
				{
					FAN_Control(ENABLE);
					fanflag = 1;
				}
				else
				{
					FAN_Control(DISABLE);
					fanflag = 0;
				}
				while(whichadkeypressed()==AD_K3);	
				SYSTICK_DelayUs(300);
			}
		}
			
		if(whichadkeypressed()==AD_K4)
		{
			SYSTICK_DelayMs(1);
			if(whichadkeypressed()==AD_K4)
			{
				if(waterflag == 0)
				{
					WATER_Control(ENABLE);
					waterflag = 1;
				}
				else
				{
					WATER_Control(DISABLE);
					waterflag = 0;
				}
				while(whichadkeypressed()==AD_K4);	
				SYSTICK_DelayUs(300);
			}
		}
		if(whichadkeypressed()==AD_K5)
		{
			SYSTICK_DelayMs(1);
			if(whichadkeypressed()==AD_K5)
			{
				if(windowflag == 0)
				{

					windowflag = 1;
				}
				else
				{

					windowflag = 0;
				}
				while(whichadkeypressed()==AD_K5);	
				SYSTICK_DelayUs(300);
			}
		}
		//ÎÂ¶È´«¸ÐÆ÷´¥·¢	
		
		Dht11_Control();
	

	
	//ÑÌÎí´«¸ÐÆ÷
	
	Mq2_Control();


//ÒÒ´¼´«¸ÐÆ÷
//		printf("ÒÒ´¼£º%d\n", ADC_GetalcoholVal());
		if(ADC_GetcoVal() >= 2500)
		{
			if(yichunflag == 0)
			{		
				printf("alcohol warning!\n");
					LED_R_flash();
				Leds_BeepCfg(BEEP ,ENABLE);
				FAN_Control(ENABLE);
					yichunflag = 1;
				}
				
			}
		else
		{
			if(yichunflag == 1)
			{
				LED_R_noflash();
					Leds_BeepCfg(BEEP ,DISABLE);
				FAN_Control(ENABLE);

					yichunflag = 0;
			}
		}

	}

	
