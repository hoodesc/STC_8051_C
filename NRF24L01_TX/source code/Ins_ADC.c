#include <STC12C2052AD.h>
#include "MYTYPE.h"
#include "Ins_ADC.h"
#include "delay.h"



uchar ADC_DATA_temp;		// ADC转换后存放在此变量“待NRF24L01发送”

void Ins_ADC_Init()
{
	P1M0 |= 0x01;	//	设置P1.0 开漏输出
	P1M1 |= 0x01;	//	设置P1.0 开漏输出
	ADC_CONTR = 0x80;	//	打开A/D转换电源
	DelayMs(1);
	ADC_CONTR = 0xE8;	//	启动转换
	EADC_SPI = 1;		//	允许A/D转换和SPI中断
	AUXR |= 0x10;		//	A/D单独中断允许位 
	EA = 1;				//	打开总中断

}

void Ins_ADC_ISR()	interrupt 5
{
	ADC_CONTR &= 0xEF;		//	清除A/D转换标识
	ADC_DATA_temp = ADC_DATA;	//	获取ADC转换结果
	ADC_CONTR |= 0x08;		//	启动A/D转换	
}