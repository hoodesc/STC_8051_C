#include <stc12c2052ad.h>
#include "MYTYPE.h"
#include "NRF24L01_TX.h"
#include "Delay.h"
#include "Ins_ADC.h"


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


void main()
{
	uchar temp = 0xa2;
	Uart_Init();		// ���ڳ�ʼ��
	Ins_ADC_Init();		// Ƭ��ADC��ʼ��
	NRF24L01_TX_Mode();	// NRF24L01����ģʽ��ʼ��
	while(1)
	{
		NRF24L01_TxPacket();
		DelayMs(10);		
//		SPI_RW_Reg(WRITE_REG + STATUS,0x2E);	// ����ж�
//		DelayMs(1);
		SBUF = adc;	// ���ڴ�ӡ
		while(!TI);
		TI = 0;
		DelayMs(100);
//
//		SPI_RW_Reg(WRITE_REG + RF_SETUP,0xf2);	// дһ�ֽ����ݵ��Ĵ���
//		DelayMs(100);
//		temp = SPI_Read(STATUS);	// ���Ĵ���һ�ֽ�����
//		SBUF = temp;	// ���ڴ�ӡ
//		while(!TI);
//		TI = 0;
//		DelayMs(100);
//
//		SPI_RW_Reg(WRITE_REG + STATUS,0x2E);	// дһ�ֽ����ݵ��Ĵ���
//
//		temp = SPI_Read(STATUS);	// ���Ĵ���һ�ֽ�����
//		SBUF = temp;	// ���ڴ�ӡ
//		while(!TI);
//		TI = 0;
//		DelayMs(100);
//		while(1);
	}
}