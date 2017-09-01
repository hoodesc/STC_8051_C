#include <stc15.h>
#include "MYTYPE.h"
#include "NRF24L01_TX.h"
#include "Delay.h"


/*  11.0592Mhz	Uart串口初始化		    */
void Uart_Init()	// 串口初始化
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}
/**********************************/


void main()
{
	uchar temp = 'k';
	Uart_Init();	// 串口初始化
	DelayMs(500);
	while(1)
	{
//		CSN = 1;	// SPI失能
//		CE  = 0;	// 芯片使能	
//		SCK = 0;	// 时钟初始化低电平

//		SPI_RW_Reg(WRITE_REG + RF_CH,0x3a);	// 写一字节数据到寄存器
//		temp = SPI_Read(READ_REG + RF_CH);	// 读寄存器一字节数据
//		SBUF = temp;	// 串口打印
//		while(!TI);
//		TI = 0;
		
		SBUF = temp;	// 串口打印
		while(!TI);
		TI = 0;		
		
		DelayMs(500);
	}
}