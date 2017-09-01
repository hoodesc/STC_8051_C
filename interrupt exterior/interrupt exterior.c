#include<stc12c2052ad.h>
//外部中断0
//P3.2按键按下，产生外部中断，外部中断0服务程序
//点亮P1.7 LED灯

typedef unsigned char uchar;
typedef unsigned int uint;

sbit Init0 = P3^2;
sbit LED1 = P1^7;


//外部中断0服务程序
void Init0_ISR(void) interrupt 0
{
	EX0 = 0;		//禁止外部中断0中断
	LED1 = ~LED1;
	EX0 =  1;
}

void main(void)
{
	IT0 = 0;	//外部中断0，低电平触发
	EX0 = 1;	//允许外部中断0中断
	EA = 1;		//开总中断
	while(1);
}