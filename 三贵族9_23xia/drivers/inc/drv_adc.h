#ifndef __DRV_ADC_H__
#define __DRV_ADC_H__

#include "stm32f10x.h"

#define NO_AD_KEY   0
#define AD_K1 1
#define AD_K2 2
#define AD_K3 3
#define AD_K4 4
#define AD_K5 5

#define AD_K1NUM    0
#define AD_K2NUM   	491.4
#define AD_K3NUM   	966.875
#define AD_K4NUM   	1279.6875
#define AD_K5NUM   	1660.135135

void ADC_Config(void);
uint16_t ADC_GetRgVal(void);
uint16_t ADC_GetAdkeyVal(void);
uint16_t ADC_GetsmokeVal(void);
int  whichadkeypressed(void);
uint16_t ADC_GetcoVal(void);

//ÑÌÎí±¨¾¯Æ÷
float MQ2_GetPPM(void);

#endif // __DRV_ADC_H__

