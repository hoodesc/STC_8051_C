#include <reg51.h>
/*
晶振：11.0592Mhz
12T
程序最终效果：
定时器0 50ms产生一次溢出且“i”自增1
当第20次溢出时，i==20，50ms*20=1s
P1取反
*/
void Timer0_Init(void)	//定时器0初始化函数
{
	TMOD = 0x01;	//设置定时器0，方式1
	TH0 = 0xD8;	    //装初值
	TL0 = 0xF0;		//定时50ms
	
	EA = 1;			//打开总中断
	ET0 = 1;		//定时器0溢出中断允许位
	TR0 = 1;		//打开定时器0
}
void Timer0_ISR(void) interrupt 1	//定时器0中断服务程序
{
	unsigned char i;
	i++;
	TH0 = 0xD8;		//重新装初值
	TL0 = 0xF0;		//定时10ms

	if(i == 100)		//10*100ms=1s
	{
		i = 0;
		P1 = ~P1;			
	}
}

void Timer1_Init(void)	//定时器1初始化函数
{
	TMOD = 0x10;	//设置定时器1，方式1
	TH1 = 0xD8;	    //装初值
	TL1 = 0xF0;		//定时50ms
	
	EA = 1;			//打开总中断
	ET1 = 1;		//定时器0溢出中断允许位
	TR1 = 1;		//打开定时器0
}
void Timer1_ISR(void) interrupt 3	//定时器0中断服务程序
{
	unsigned char i;
	i++;
	TH1 = 0xD8;		//重新装初值
	TL1 = 0xF0;		//定时10ms

	if(i == 100)		//10*100ms=1s
	{
		i = 0;
		P3 = ~P3;	
	}
}

void main(void)
{
	Timer0_Init();	//调用定时器0初始化函数
	//Timer1_Init();
	while(1)
	{}	
}