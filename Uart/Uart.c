#include <stc15.h>
#include <stdio.h>

void Uart_Init()
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器2时钟为Fosc/12,即12T
	T2L = 0xFE;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

void delay()
{
	unsigned char i,j;
	for(i = 0; i < 255; i++)
		for(j = 0; j < 255; j++);
}

void main()
{
	Uart_Init();
	delay();
	while(1)
	{
//		TI = 1;
//		printf("a\n");
//		delay();
//		while(!TI);
//		TI = 0;	
			SBUF = 0x1a;
			while(!TI);
	    TI = 0;
			delay();
	}
}