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

	j = 0x08 - addr % 0x08;		//��ǰҳ��ʣ����

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

	if(j >= n)		//��д���ݵ�ǰҳ��װ��
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

	else	//��д���ݵ�ǰҳװ����д���跭ҳд
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

		addr += j;		//��ҳ��ַ
		a = n-j;		//������ʣ����
		while(a >= 8)	//��ʣ������װ��һҳ
		{
			j += 8;
			SYSTICK_DelayMs(10);		//��ҳ��ʱ
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

		//д��ʣ��װ����һҳ������
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
		if(0xff == addr)		//�������������һ����ַ
		{
			data[i] = IIC_recdata(IIC_NOACK);
			break;
		}
		else
		{
			if(n-1 == i)		//��ȡ���һ�����ݷ��ͷ�Ӧ��
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
