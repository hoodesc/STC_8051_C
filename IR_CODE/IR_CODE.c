//#include <stc12c2052ad.h>
#include <stc15.h>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;

uchar code IR_DATA1[] = {0xB2,0x4D,0x1F,0xE0,0x94,0x6B,0xff};	// 红外数据：
//uchar code IR_DATA2[] = {0xB2,0x4D,0x3F,0xC0,0x20,0xDF,0xff};	// 红外数据：20度
////
//uchar code IR_DATA3[] = {0xB2,0x4D,0x3F,0xC0,0x40,0xBF,0xff};	// 红外数据：24度
////							  0011 1111  0100 0000
//uchar code IR_DATA4[] = {0xB2,0x4D,0x3F,0xC0,0x20,0xDF,0xff};	// 红外数据：20度
////							  0011 1111  0010 0000							  0011 1111  0010 0000
//sbit IR = P1^7;
sbit IR = P3^4;
uchar IR_Code_Flag;
uint IR_Code_Time = 0; 
// 定时器0 产生38KHz方波
// 26us周期、13us中断一次IR取反

//函数声明
void Stop(void);


void Timer0_Init()
{
	TMOD &= 0xF0;
	TMOD |= 0x02;	// 定时器0 8位自动重装模式
	TL0 = 0xF4;		// 设置定时初值  13us
	TH0 = 0xF4;		// 设置定时重载值
	ET0   = 1;	
	TR0   = 1;
	EA    = 1;	
}

void Timer0_ISR() interrupt 1
{
	IR_Code_Time++;
	if(IR_Code_Flag)
	{
		IR = ~IR;	
	}
	else
	{
		IR = 1;
	}
}

void IR_Code(uchar *pIR_Dat)
{
	uchar i,j,temp;	
	// 4.4ms(38KHz)
	IR_Code_Time = 0;	// 时间清零	
	IR_Code_Flag = 1;	// 产生38KHz载波	
	while(IR_Code_Time < 338);

	// 4.4ms(低电平)	
	IR_Code_Time = 0;	// 时间清零
	IR_Code_Flag = 0;	// 产生低电平
	while(IR_Code_Time < 338);

	for(i = 0; i < 6; i++)
	{
		temp = *pIR_Dat;
		for(j = 0; j < 8; j++)
		{
			if(temp & 0x80)
			{
				// 数据1
				// 540us(38KHz载波)+1.62ms(低电平)
				IR_Code_Time = 0;	// 时间清零				
				IR_Code_Flag = 1;	// 产生38KHz载波					
				while(IR_Code_Time < 41);

				IR_Code_Time = 0;	// 时间清零
				IR_Code_Flag = 0;	// 产生低电平
				while(IR_Code_Time < 124);
			}
			else
			{  	
				// 数据0
				// 540us(38KHz载波)+560(低电平)				
				IR_Code_Time = 0;	// 时间清零					
				IR_Code_Flag = 1;	// 产生38KHz载波
				while(IR_Code_Time < 41);

 				IR_Code_Time = 0;	// 时间清零
				IR_Code_Flag = 0;	// 产生低电平
				while(IR_Code_Time < 41);
			}
			temp<<=1;			
		}
		pIR_Dat++;
	}
}

void Stop(void)
{
	//结束、分隔	
	IR_Code_Time = 0;	// 时间清零
	IR_Code_Flag = 1;	// 产生38KHz载波
	while(IR_Code_Time < 41);

	IR_Code_Time = 0;	// 时间清零
	IR_Code_Flag = 0;	// 产生低电平	
	while(IR_Code_Time < 401);
}
//	11.0592延时函数，单位ms
void DelayMs(uint ValMS)
{
	uint uiVal,ujVal;
	for(uiVal = 0; uiVal < ValMS; uiVal++)
		for(ujVal = 0; ujVal < 120; ujVal++);
}
void main(void)
{
	Timer0_Init();
	P1M1 = 0x00;
	P1M0 = 0x00;
	DelayMs(100);
	while(1)
	{
	IR_Code(IR_DATA1);
	Stop();
	IR_Code(IR_DATA1);
	Stop();
	while(1);
	}
}