#include <stc12c2052ad.h>
#include <intrins.h>
#include "MYTYPE.h"
#include "Delay.h"
#include "Ins_ADC.h"

uchar ADC_DATA_temp[] = {0xaa,0x11,0x22,0x33};
uchar adc;
void Ins_ADC_Init()
{
	P1M0 = 0x01;	// P1^0 ��©���
	P1M1 = 0x01;	// P1^0 ��©���
	ADC_CONTR = 0xE8;	//	��ADC��Դ��270��ʱ������ת��һ�Ρ�ת����ɱ�ʶ0��������ͨ������
	_nop_();_nop_();_nop_();_nop_();	// �ȴ�4����������
	EADC_SPI = 1;	// ����A/Dת���жϺ�SPI�ж�	
	AUXR |= 0x10;	// ����A/Dת���ж�
	EA = 1;			// �����ж� 
}

void Ins_ADC_ISR()	interrupt 5
{
	ADC_CONTR &= 0xEF;		// ���A/Dת��������ʶ
	adc = ADC_DATA;	// ��ȡת�����
	ADC_CONTR |= 0x08;		// �ٴ�����A/Dת��
}


//void main(void)
//{
//	while(1)
//	{
//		SBUF = ADC_DATA_temp;
//		while(!TI);
//		TI = 0;
//		DelayMs(10);
//	}
//}