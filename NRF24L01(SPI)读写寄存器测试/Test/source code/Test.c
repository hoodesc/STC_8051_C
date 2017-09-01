#include <stc15.h>
#include "MYTYPE.h"
#include "NRF24L01_TX.h"
#include "Delay.h"


/*  11.0592Mhz	Uart���ڳ�ʼ��		    */
void Uart_Init()	// ���ڳ�ʼ��
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xBF;		//��ʱ��1ʱ��ΪFosc/12,��12T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFD;		//�趨��ʱ��ֵ
	TH1 = 0xFD;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
}
/**********************************/


void main()
{
	uchar temp = 'k';
	Uart_Init();	// ���ڳ�ʼ��
	DelayMs(500);
	while(1)
	{
//		CSN = 1;	// SPIʧ��
//		CE  = 0;	// оƬʹ��	
//		SCK = 0;	// ʱ�ӳ�ʼ���͵�ƽ

//		SPI_RW_Reg(WRITE_REG + RF_CH,0x3a);	// дһ�ֽ����ݵ��Ĵ���
//		temp = SPI_Read(READ_REG + RF_CH);	// ���Ĵ���һ�ֽ�����
//		SBUF = temp;	// ���ڴ�ӡ
//		while(!TI);
//		TI = 0;
		
		SBUF = temp;	// ���ڴ�ӡ
		while(!TI);
		TI = 0;		
		
		DelayMs(500);
	}
}