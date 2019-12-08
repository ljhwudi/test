#ifndef __DRV_SYSTICK_H__
#define __DRV_SYSTICK_H__

#include "stm32f10x.h"

void SYSTICK_Config(uint8_t sysclk);
void SYSTICK_DelayMs(uint16_t ms);
void SYSTICK_DelayUs(uint16_t us);

#endif // __DRV_SYSTICK_H__


