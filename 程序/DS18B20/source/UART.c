#include <stc15.h>
#include "mytype.h"
#include "uart.h"

/*  11.0592Mhz	Uart串口初始化		    */
void Uart_Init()	// 串口初始化
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器2时钟为Fosc/12,即12T
	T2L = 0xE8;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}
/**********************************/

/**********************************/
//	串口调试
//void Uart_print(uchar *pdat,uchar len)
//{
//	uchar i;
//	for(i = 0; i < len; i++)
//	{
//		SBUF = pdat[i];
//		while(!TI);
//		TI = 0;		
//	}
//}