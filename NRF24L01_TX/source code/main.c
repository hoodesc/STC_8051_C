#include <STC15.h>
#include "MYTYPE.h"
#include "Delay.h"
#include "NRF24L01_TX.h"
#include "main.h"
#include "keyscan.h"


/*  11.0592Mhz	Uart���ڳ�ʼ��		    */
void Uart_Init()	// ���ڳ�ʼ��
{
	TMOD &= 0x0F;	// ������ʱ��0���ã���ն�ʱ��1����
	TMOD |= 0x20;	// ��ʱ��1 ģʽ2
	SCON |= 0x50;	// ���ڷ�ʽ1��8λUART�����ʿɱ䣬�ɽ���
	TH1   = 0xFD;	// TL1�����TH1��ֵװ��
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// �رն�ʱ��1�ж�
	TR1   = 1;		// ����ʱ��1
}
/**********************************/


//	���ڵ���
//void Uart_print(void)
//{
//	uchar temp = 0;
//	temp = SPI_Read(0x00);
//	SBUF = temp;
//	while(!TI);
//	TI = 0;
//}

void Timer0_Init(void)
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
	TR0 = 1;
}

void main()
{
//	Uart_Init();
	P3M1 = 0x00;
	P3M0 = 0x00;
	Timer0_Init();
	SetTX_Mode();
	DelayMs(500);
	while(1)
	{
		if(TF0)
		{
			TF0 = 0;
			TL0 = 0x00;		//���ö�ʱ��ֵ
			TH0 = 0xDC;		//���ö�ʱ��ֵ
			Keyscan();	
		}

//	SPI_RW_Reg(WRITE_REG + STATUS,0x7E);				//	���״̬�Ĵ���
//	SPI_Write_Buf(W_TX_PAYLOAD,TX_DAT,TX_PLOAD_WIDTH);	//	װ������
//	NRF24L01_TxPacket(TX_DAT);		//	����
//	while(1);
//	//	�������ݺ��ӡstatus��fifo_status�Ĵ���
//	while(1);
	}
}