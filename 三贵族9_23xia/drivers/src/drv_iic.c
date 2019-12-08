#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "drv_systick.h"
#include "drv_iic.h"


#define IIC_SCL			GPIO_Pin_6
#define IIC_SDA			GPIO_Pin_7

#define SCL_HIGHT()		GPIO_SetBits(GPIOB, IIC_SCL)	
#define SCL_LOW()		GPIO_ResetBits(GPIOB, IIC_SCL)

#define SDA_HIGHT()		GPIO_SetBits(GPIOB, IIC_SDA)
#define SDA_LOW()		GPIO_ResetBits(GPIOB, IIC_SDA)
#define SDA_READ()		GPIO_ReadInputDataBit(GPIOB, IIC_SDA)

void IIC_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStruct.GPIO_Pin = IIC_SCL | IIC_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}


void IIC_SDAin(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = IIC_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void IIC_SDAout(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = IIC_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void IIC_start(void)
{
	SCL_LOW();
	IIC_SDAout();
	
	SDA_HIGHT();
	SCL_HIGHT();
	SYSTICK_DelayUs(5);
	SDA_LOW();
	SCL_LOW();
}

void IIC_stop(void)
{
	SCL_LOW();
	IIC_SDAout();

	SDA_LOW();
	SCL_HIGHT();
	SYSTICK_DelayUs(5);
	SDA_HIGHT();
}

void IIC_sendACK(void)
{
	SCL_LOW();
	IIC_SDAout();

	SDA_LOW();
	SCL_HIGHT();
	SYSTICK_DelayUs(5);
	SCL_LOW();
	SDA_HIGHT();
}

void IIC_noACK(void)
{
	SCL_LOW();
	IIC_SDAout();

	SDA_HIGHT();
	SCL_HIGHT();
	SYSTICK_DelayUs(5);
	SCL_LOW();
}

uint8_t IIC_waitACK(void)
{
	uint8_t i=0;

	SCL_LOW();
	IIC_SDAin();
	
	SCL_HIGHT();
	SYSTICK_DelayUs(2);
	while(SDA_READ())
	{
		if(255==i)
		{
			IIC_stop();
			return IIC_NOACK;
		}
		i++;
	}

	SCL_LOW();
	return IIC_ACK;
}

void IIC_senddata(uint8_t data)
{
	uint8_t i;

	SCL_LOW();
	IIC_SDAout();

	for(i=0; i<8 ;i++)
	{
		if(data & 0x80)
			SDA_HIGHT();
		else
			SDA_LOW();
		
		data <<= 1;
		SCL_HIGHT();
		SYSTICK_DelayUs(5);
		SCL_LOW();
		SYSTICK_DelayUs(5);
	}
}

uint8_t IIC_recdata(uint8_t ackflag)
{
	uint8_t i;
	uint8_t data=0;

	SCL_LOW();
	IIC_SDAin();

	for(i=0; i<8 ;i++)
	{
		SCL_HIGHT();
		SYSTICK_DelayUs(2);
		data <<= 1;
		if(SDA_READ())
			data |= 0x01;
		SCL_LOW();
		SYSTICK_DelayUs(2);
	}

	if(IIC_ACK == ackflag)
		IIC_sendACK();
	else
		IIC_noACK();

	return data;
}
