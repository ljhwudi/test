#ifndef __DRV_IIC_H__
#define __DRV_IIC_H__

#include "stm32f10x.h"

#define IIC_ACK		1
#define IIC_NOACK	0


void IIC_Config(void);
void IIC_SDAin(void);
void IIC_SDAout(void);
void IIC_start(void);
void IIC_stop(void);
void IIC_sendACK(void);
void IIC_noACK(void);
uint8_t IIC_waitACK(void);
void IIC_senddata(uint8_t data);
uint8_t IIC_recdata(uint8_t ackflag);

#endif //__DRV_IIC_H__
