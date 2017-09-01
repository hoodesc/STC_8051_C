#include <stc12c2052ad.h>
#include "MYTYPE.h"
#include "NRF24L01_RX.h"
#include "Delay.h"
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
void Uart_print(void)
{
	uchar temp = 0;
	temp = SPI_Read(CD);
	SBUF = temp;
	while(!TI);
	TI = 0;
}


void main()
{	
	Uart_Init();		//	���ڳ�ʼ��
	NRF24L01_Init();	//	NRF24L01��ʼ��
	DelayMs(1000);
	while(1)
	{
//		SetRX_Mode();
//		NRF24L01_RxPacket(RX_DAT);
		Uart_print();
//		motor();
		
	}
}