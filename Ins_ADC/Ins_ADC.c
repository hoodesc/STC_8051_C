#include <stc12c2052ad.h>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;

uchar ADC_DATA_temp;
//	11.0592延时函数，单位ms
void DelayMs(uint ValMS)
{
	uint uiVal,ujVal;
	for(uiVal = 0; uiVal < ValMS; uiVal++)
		for(ujVal = 0; ujVal < 120; ujVal++);
}

void Ins_ADC_Init()
{
	P1M0 = 0x01;	// P1^0 开漏输出
	P1M1 = 0x01;	// P1^0 开漏输出
	ADC_CONTR = 0xE8;
	_nop_();_nop_();_nop_();_nop_();	// 等待4个
	EADC_SPI = 1;	// 允许A/D转换中断和SPI中断	
	AUXR |= 0x10;	// 允许A/D转换中断
	EA = 1;			// 打开总中断 
}

void Ins_ADC_ISR()	interrupt 5
{
	ADC_CONTR &= 0xEF;		// 清除A/D转换结束标识
	ADC_DATA_temp = ADC_DATA;
	ADC_CONTR |= 0x08;		// 启动A/D转换
}

 /*  11.0592Mhz	Uart串口初始化		    */
void Uart_Init()	// 串口初始化
{
	TMOD &= 0x0F;	// 保留后定时器0设置，清空定时器1设置
	TMOD |= 0x20;	// 定时器1 模式2
	SCON |= 0x50;	// 串口方式1，8位UART波特率可变，可接收
	TH1   = 0xFD;	// TL1溢出后将TH1的值装入
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// 关闭定时器1中断
	TR1   = 1;		// 开定时器1
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