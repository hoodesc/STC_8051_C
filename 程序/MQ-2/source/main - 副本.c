#include <STC15.h> 
#include "MYTYPE.h"
#include "Delay.h"
#include "UART.h"
#include "nrf24l01_tx.h"

//	MQ-2烟雾传感器
//	NRF24L01 频道30
//	收发地址：0x12,0x21,0xaa,0x34,0x43
//	设备码A1

sbit MQ2 = P3^2;
uchar tmp = 0;

void INT_Init()
{
	IT0 = 1;	//外部中断0 下降沿触发
	EX0 = 1;	//使能外部中断0
}

void Timer0_Inti()	//1ms@11.0592MHz
{
	AUXR &= 0x7F;
	TMOD &= 0xF0;		//16位自动重装
	TL0 = 0x66;
	TH0 = 0xFC;
	ET0 = 1;
	EA  = 1;
	TR0 = 1;
}

void INT0_ISR()	interrupt 0
{		
		tmp++;
		if (tmp == 1)
		{
			Timer0_Inti();
			DelayUs(1);
			MQ2 = 1; 
			DelayUs(1);
		} 
		else if(tmp == 2)
		{
			EX0 = 0;
		}
}

void Timer0_ISR()	interrupt 1
{
	static uint time = 0;
	time++;
	if(time > 1000)//每秒检测一次警报是否消除
	{
			if(MQ2)
				TX_DAT[1] = 0xBB;
			else
			{
				TX_DAT[1] = 0xAA;
				tmp=0;
				EX0 = 1;
			}
				time =0;
	}
}


//sbit led = P3^4;
void main(void)
{
	uchar i = 0;
	P1M0 = 0x00;
	P1M1 = 0x00;
	SetTX_Mode();
	DelayMs(500);
	INT_Init();
	Timer0_Inti();
	while(1)
	{
//		if(i < 1)
//		{
//			TX_DAT[1] = 0xAA;
//						i++;
//		}
//		else
//		{
//			TX_DAT[1] = 0xBB;		
//			i = 0;		
//		}

		SPI_RW_Reg(WRITE_REG + STATUS,0x7E);				//	清除状态寄存器
		SPI_Write_Buf(W_TX_PAYLOAD,TX_DAT,TX_PLOAD_WIDTH);	//	装载数据
		NRF24L01_TxPacket(TX_DAT);		//	发送
		DelayMs(800);
	}		
}