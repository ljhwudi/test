#include "drv_iic.h"
#include "drv_systick.h"
#include <stdio.h>

void AT24CXX_ByteWrite(uint8_t addr, uint8_t data)
{
	IIC_start();
	IIC_senddata(0xA0);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the device addr err!\n");
		return ;
	}
	
	IIC_senddata(addr);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the word addr err!\n");
		return ;
	}
	
	IIC_senddata(data);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the data rec err!\n");
		return ;
	}
	
	IIC_stop();
}

uint8_t AT24CXX_RandomRead(uint8_t addr)
{
	uint8_t data=0;

	IIC_start();
	IIC_senddata(0xA0);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the device addr err!\n");
		return 0;
	}
	
	IIC_senddata(addr);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the word addr err!\n");
		return 0;
	}
	
	IIC_start();
	IIC_senddata(0xA1);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the device addr err!\n");
		return 0;
	}
	
	data = IIC_recdata(IIC_NOACK);
	IIC_stop();

	return data;
}

void AT24CXX_PageWrite(uint8_t addr, uint8_t *data, uint8_t n)
{
	uint8_t i=0,j=0;

	j = 0x08 - addr % 0x08;		//当前页所剩容量

	IIC_start();
	IIC_senddata(0xA0);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the device addr err!\n");
		return ;
	}
	
	IIC_senddata(addr);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the word addr err!\n");
		return ;
	}

	if(j >= n)		//所写内容当前页能装下
	{
		for(i=0; i<n; i++)
		{
			IIC_senddata(data[i]);
			if(IIC_ACK != IIC_waitACK())
			{
				printf("the data rec err!\n");
				return ;
			}
		}
		IIC_stop();
		return ;
	}

	else	//所写内容当前页装不下写完需翻页写
	{	
		uint8_t a=0;
	
		for(i=0; i<j; i++)
		{
			IIC_senddata(data[i]);
			if(IIC_ACK != IIC_waitACK())
			{
				printf("the data rec err!\n");
				return ;
			}
		}
		IIC_stop();

		addr += j;		//新页地址
		a = n-j;		//内容所剩个数
		while(a >= 8)	//所剩内容能装满一页
		{
			j += 8;
			SYSTICK_DelayMs(10);		//换页延时
			IIC_start();
			IIC_senddata(0xA0);
			if(IIC_ACK != IIC_waitACK())
			{
				printf("the device addr err!\n");
				return ;
			}

			IIC_senddata(addr);
			if(IIC_ACK != IIC_waitACK())
			{
				printf("the word addr err!\n");
				return ;
			}
			
			for(i=i; i<j ; i++)
			{
				IIC_senddata(data[i]);
				if(IIC_ACK != IIC_waitACK())
				{
					printf("the data rec err!\n");
					return ;
				}
			}
			IIC_stop();
			addr += 8;
			a -= 8;
		}

		//写完剩下装不满一页的内容
		SYSTICK_DelayMs(10);
		IIC_start();
		IIC_senddata(0xA0);
		if(IIC_ACK != IIC_waitACK())
		{
			printf("the device addr err!\n");
			return ;
		}

		IIC_senddata(addr);
		if(IIC_ACK != IIC_waitACK())
		{
			printf("the word addr err!\n");
			return ;
		}
		
		for(i=i; i<n; i++)
		{
			IIC_senddata(data[i]);
			if(IIC_ACK != IIC_waitACK())
			{
				printf("the data rec err!\n");
				return ;
			}
		}
		IIC_stop();
	}
}

void AT24CXX_SeqRead(uint8_t addr, uint8_t *data, uint8_t n)
{
	uint8_t i=0;

	IIC_start();
	IIC_senddata(0xA0);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the device addr err!\n");
		return ;
	}
	
	IIC_senddata(addr);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the word addr err!\n");
		return ;
	}
	
	IIC_start();
	IIC_senddata(0xA1);
	if(IIC_ACK != IIC_waitACK())
	{
		printf("the device addr err!\n");
		return ;
	}

	for(i=0; i<n; i++)
	{
		if(0xff == addr)		//所读内容在最后一个地址
		{
			data[i] = IIC_recdata(IIC_NOACK);
			break;
		}
		else
		{
			if(n-1 == i)		//读取最后一个数据发送非应答
				data[i] = IIC_recdata(IIC_NOACK);
			else
				data[i] = IIC_recdata(IIC_ACK);
			addr++;
			if(0xff < addr)
				break;
		}
	}

	IIC_stop();
}
