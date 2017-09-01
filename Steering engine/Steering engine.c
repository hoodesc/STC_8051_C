#include<STC12C2052AD.h>
typedef unsigned int uint;
typedef unsigned char uchar;
sbit PWM=P1^7;//输出PWM信号

void Timer0_Init(void)		//定时器0初始化
{
	TMOD = 0x01;
	TH0 = 0xF8;	//装初值
	TL0 = 0x30;	//定时2ms
	ET0 = 1;
	EA = 0;
	TR0 = 0;	
}

void Timer0_ISR(void) interrupt 1	//定时器0 中断服务函数
{
	static uchar i = 0;
	i++;
	TR0 = 0;
	if(1 == i%2)	//奇数次，高电平
	{
		PWM = 1;
		TH0 = 0xB9;	//重装18ms初值
		TL0 = 0xB0;	
		TR0 = 1;	//启动定时器
	
	}
	if(0 == i%2)	//偶数次，低电平
	{
		PWM = 0;	
		TH0 = 0xF8;	////重装2ms初值
		TL0 = 0x30;
		TR0 = 1;	//启动定时器
		i = 2;
	}
}
void main(void)//主函数
{
	P1M0 = 0x00;
	P1M1 = 0xFF;
	Timer0_Init();
	while(1)
	{}
}