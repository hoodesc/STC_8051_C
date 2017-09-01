#include <stc12c2052ad.h>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;

uchar ADC_DATA_temp;
//	11.0592��ʱ��������λms
void DelayMs(uint ValMS)
{
	uint uiVal,ujVal;
	for(uiVal = 0; uiVal < ValMS; uiVal++)
		for(ujVal = 0; ujVal < 120; ujVal++);
}

void Ins_ADC_Init()
{
	P1M0 = 0x01;	// P1^0 ��©���
	P1M1 = 0x01;	// P1^0 ��©���
	ADC_CONTR = 0xE8;
	_nop_();_nop_();_nop_();_nop_();	// �ȴ�4��
	EADC_SPI = 1;	// ����A/Dת���жϺ�SPI�ж�	
	AUXR |= 0x10;	// ����A/Dת���ж�
	EA = 1;			// �����ж� 
}

void Ins_ADC_ISR()	interrupt 5
{
	ADC_CONTR &= 0xEF;		// ���A/Dת��������ʶ
	ADC_DATA_temp = ADC_DATA;
	ADC_CONTR |= 0x08;		// ����A/Dת��
}

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

void main(void)
{
	Ins_ADC_Init();
	Uart_Init();
	while(1)
	{
		SBUF = ADC_DATA_temp;
		while(!TI);
		TI = 0;
		DelayMs(100);
	}
}