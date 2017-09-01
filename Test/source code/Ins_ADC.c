#include <stc12c2052ad.h>
#include <intrins.h>
#include "MYTYPE.h"
#include "Delay.h"
#include "Ins_ADC.h"

uchar ADC_DATA_temp[] = {0xaa,0x11,0x22,0x33};
uchar adc;
void Ins_ADC_Init()
{
	P1M0 = 0x01;	// P1^0 开漏输出
	P1M1 = 0x01;	// P1^0 开漏输出
	ADC_CONTR = 0xE8;	//	开ADC电源、270个时钟周期转换一次、转换完成标识0、启动、通道设置
	_nop_();_nop_();_nop_();_nop_();	// 等待4个机器周期
	EADC_SPI = 1;	// 允许A/D转换中断和SPI中断	
	AUXR |= 0x10;	// 允许A/D转换中断
	EA = 1;			// 打开总中断 
}

void Ins_ADC_ISR()	interrupt 5
{
	ADC_CONTR &= 0xEF;		// 清除A/D转换结束标识
	adc = ADC_DATA;	// 获取转换结果
	ADC_CONTR |= 0x08;		// 再次启动A/D转换
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