#include <STC15.h> 
#include "MYTYPE.h"
#include "Delay.h"
#include "UART.h"
#include "nrf24l01_tx.h"

//	MQ-2��������
//	NRF24L01 Ƶ��30
//	�շ���ַ��0x12,0x21,0xaa,0x34,0x43
//	�豸��A1

sbit MQ2 = P3^2;

void INT_Init()
{
	IT0 = 1;	//�ⲿ�ж�0 �½��ش���
	EX0 = 1;	//ʹ���ⲿ�ж�0
}

void Timer0_Inti()	//1ms@11.0592MHz
{
	AUXR &= 0x7F;
	TMOD &= 0xF0;		//16λ�Զ���װ
	TL0 = 0x66;
	TH0 = 0xFC;
	ET0 = 1;
	EA  = 1;
	TR0 = 0;
}

void INT0_ISR()	interrupt 0
{
		TR0 = 1;	//��ʼ��ʱ
		EX0 = 0;	//�ر��ⲿ�ж�0	
}

void Timer0_ISR()	interrupt 1
{
	static uint time = 0;
	time++;
	if(time > 500)//ÿ����һ�ξ����Ƿ�����
	{
		if(MQ2)
		{
			TX_DAT[1] = 0xAA;
			EX0 = 1;
			TR0 = 0;
		}
		else
		{
			TX_DAT[1] = 0xBB;
			EX0 = 0;
		}
			time = 0;
	}
}

void main(void)
{
	uchar i = 0;
	P1M0 = 0x00;
	P1M1 = 0x00;
	INT_Init();
	Timer0_Inti();
	SetTX_Mode();
	DelayMs(500);
	while(1)
	{
		SPI_RW_Reg(WRITE_REG + STATUS,0x7E);				//	���״̬�Ĵ���
		SPI_Write_Buf(W_TX_PAYLOAD,TX_DAT,TX_PLOAD_WIDTH);	//	װ������
		NRF24L01_TxPacket(TX_DAT);		//	����
		DelayMs(50);
	}		
}