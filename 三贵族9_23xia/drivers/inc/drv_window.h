#ifndef _DRV_WINDOW_H_
#define _DRV_WINDOW_H_

#include "stm32f10x.h"

void WINDOW_Init(void);
//void WINDOW_PinControl(uint16_t GPIO_Pin,FunctionalState NewState);
void WINDOW_Open(u8 angle);
//void PIN_Cfg(u8 t);



#endif	//_DRV_WINDOW_H_

