#include "stm32f10x.h"

static uint8_t fs_us;
static uint16_t fs_ms;

void SYSTICK_Config(uint8_t sysclk)
{
	// 确定时钟源
	SysTick->CTRL &= ~0x05;

	fs_us = sysclk / 8;
	fs_ms = fs_us * 1000;
}

void SYSTICK_DelayMs(uint16_t ms)
{
	uint32_t temp;
	while(ms>1864)
	{
		SysTick->LOAD = fs_ms * 1864;
		SysTick->VAL = 0;
		SysTick->CTRL |= 0x01;
		
		do {
			temp = SysTick->CTRL;
		} while ((temp&0x01) && (!(temp & (1<<16))));
		ms -= 1864;
	}
	SysTick->LOAD = fs_ms * ms;
	SysTick->VAL = 0;
	SysTick->CTRL |= 0x01;

	do {
		temp = SysTick->CTRL;
	} while ((temp&0x01) && (!(temp & (1<<16))));

	// 失能systick
	SysTick->CTRL &= ~0x01;
}

void SYSTICK_DelayUs(uint16_t us)
{
	uint32_t temp;
	
	SysTick->LOAD = fs_us * us;
	SysTick->VAL = 0;
	SysTick->CTRL |= 0x01;

	do {
		temp = SysTick->CTRL;
	} while ((temp&0x01) && (!(temp & (1<<16))));

	// 失能systick
	SysTick->CTRL &= ~0x01;
}


