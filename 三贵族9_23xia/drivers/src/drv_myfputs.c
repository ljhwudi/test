#include "stm32f10x_usart.h"

void myfputs(char *ch)
{
	while(*ch != '\0')
	{
		while (SET != USART_GetFlagStatus(USART1, USART_FLAG_TXE));
		USART_SendData(USART1, *ch);
		ch++;
	}
}
