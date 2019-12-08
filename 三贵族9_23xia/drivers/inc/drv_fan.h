#ifndef _DRV_FAN_H_
#define _DRV_FAN_H_

#include "stm32f10x.h"



void FAN_Init(void);
void FAN_Control(FunctionalState NewState);


#endif	//_DRV_FAN_H_

