#include <stc15.h>
#include "mytype.h"
#include "uart.h"
#include "a6.h"



void UART1_SendData(uchar dat)
{
	ES=0;					//关串口中断
	SBUF=dat;			
	while(TI!=1);	//等待发送成功
	TI=0;					//清除发送中断标志
	ES=1;					//开串口中断
}

void UART1_SendString(char *s)
{
	while(*s)//检测字符串结束符
	{
		UART1_SendData(*s++);//发送当前字符
	}
}


/********************* UART1中断函数************************/
void UART1_int (void) interrupt 4
{
	if (RI)
    {
      RI = 0;                           //清除RI位
			Uart1_Buf[First_Int] = SBUF;  	  //将接收到的字符串存到缓存中
			First_Int++;                			//缓存指针向后移动
			if(First_Int > Buf1_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
			{
				First_Int = 0;
			}
    }
    if (TI)
    {
        TI = 0;                          //清除TI位
    }
}

void USART1_Init(void)
{
	SCON = 0x50;
	AUXR |= 0x01;
	AUXR &= 0xFB;	
	T2L = 0xFE;
	T2H = 0xFF;
	AUXR |= 0x10;	
	ES  = 1;
	EA = 1;
}