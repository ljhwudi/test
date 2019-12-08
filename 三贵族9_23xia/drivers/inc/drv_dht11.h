#ifndef __DRV_DHT11_H
#define	__DRV_DHT11_H

#include "stm32f10x.h"
#include "drv_SysTick.h"

#define HIGH  1
#define LOW   0

#define DHT11_CLK     RCC_APB2Periph_GPIOA
#define DHT11_PIN     GPIO_Pin_8                  
#define DHT11_PORT		GPIOA 

//���κ꣬��������������һ��ʹ��,����ߵ�ƽ��͵�ƽ
#define DHT11_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_8);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_8)
 //��ȡ���ŵĵ�ƽ
#define  DHT11_DATA_IN()	   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)

typedef struct
{
	uint8_t  humi_int;		//ʪ�ȵ���������
	uint8_t  humi_deci;	 	//ʪ�ȵ�С������
	uint8_t  temp_int;	 	//�¶ȵ���������
	uint8_t  temp_deci;	 	//�¶ȵ�С������
	uint8_t  check_sum;	 	//У���
		                 
}DHT11_Data_TypeDef;

static DHT11_Data_TypeDef DHT11_Data;

void DHT11_GPIO_Config(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);
void Dht11_Control(void);

#endif /* __DHT11_H */







