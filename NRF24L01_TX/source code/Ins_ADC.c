#include <STC12C2052AD.h>
#include "MYTYPE.h"
#include "Ins_ADC.h"
#include "delay.h"



uchar ADC_DATA_temp;		// ADCת�������ڴ˱�������NRF24L01���͡�

void Ins_ADC_Init()
{
	P1M0 |= 0x01;	//	����P1.0 ��©���
	P1M1 |= 0x01;	//	����P1.0 ��©���
	ADC_CONTR = 0x80;	//	��A/Dת����Դ
	DelayMs(1);
	ADC_CONTR = 0xE8;	//	����ת��
	EADC_SPI = 1;		//	����A/Dת����SPI�ж�
	AUXR |= 0x10;		//	A/D�����ж�����λ 
	EA = 1;				//	�����ж�

}

void Ins_ADC_ISR()	interrupt 5
{
	ADC_CONTR &= 0xEF;		//	���A/Dת����ʶ
	ADC_DATA_temp = ADC_DATA;	//	��ȡADCת�����
	ADC_CONTR |= 0x08;		//	����A/Dת��	
}