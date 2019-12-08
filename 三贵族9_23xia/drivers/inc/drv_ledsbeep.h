#ifndef __DRV_LEDSBEEP_H__
#define __DRV_LEDSBEEP_H__

#include "stm32f10x.h"

#define LED_RED    GPIO_Pin_8
#define LED_GREEN  GPIO_Pin_7
#define LED_BULE   GPIO_Pin_6
#define BEEP       GPIO_Pin_9

void Leds_BeepCfg(uint16_t GPIO_Pin,FunctionalState NewState);



#endif
