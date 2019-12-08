#include "gizwits_product.h"
#include "gizwits_protocol.h"
#include "common.h"
#include "dataPointTools.h"
#include "ringBuffer.h"

#include "stm32f10x.h"
#include "drv_led.h"
#include "drv_K1.h"
#include "drv_systick.h"
#include "drv_uart.h"
#include "drv_tim.h"

#define PROTOCOL_DEBUG

int main(int argc, char *argv[])
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	SYSTICK_Config(72);
	UART1_Config();
	UART2_Config();
	TIM6_config();
	LED_Config();
	K1_Config();

	userInit();
	gizwitsInit();
	//gizwitsSetMode(WIFI_AIRLINK_MODE);

	while(1)
	{
		
		//K1_wifilogin();
		//SYSTICK_DelayMs(1000);
		userHandle();
		gizwitsHandle((dataPoint_t *)&currentDataPoint);
	}
	
}
