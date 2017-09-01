#include <stc12c2052ad.h>
#include "MYTYPE.h"
#include "NRF24L01_TX.h"
#include "Delay.h"
#include "Ins_ADC.h"


/*  11.0592Mhz	Uart串口初始化		    */
void Uart_Init()	// 串口初始化
{
	TMOD &= 0x0F;	// 保留定时器0设置，清空定时器1设置
	TMOD |= 0x20;	// 定时器1 模式2
	SCON |= 0x50;	// 串口方式1，8位UART波特率可变，可接收
	TH1   = 0xFD;	// TL1溢出后将TH1的值装入
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// 关闭定时器1中断
	TR1   = 1;		// 开定时器1
}
/**********************************/


void main()
{
	uchar temp = 0xa2;
	Uart_Init();		// 串口初始化
	Ins_ADC_Init();		// 片内ADC初始化
	NRF24L01_TX_Mode();	// NRF24L01发送模式初始化
	while(1)
	{
		NRF24L01_TxPacket();
		DelayMs(10);		
//		SPI_RW_Reg(WRITE_REG + STATUS,0x2E);	// 清除中断
//		DelayMs(1);
		SBUF = adc;	// 串口打印
		while(!TI);
		TI = 0;
		DelayMs(100);
//
//		SPI_RW_Reg(WRITE_REG + RF_SETUP,0xf2);	// 写一字节数据到寄存器
//		DelayMs(100);
//		temp = SPI_Read(STATUS);	// 读寄存器一字节数据
//		SBUF = temp;	// 串口打印
//		while(!TI);
//		TI = 0;
//		DelayMs(100);
//
//		SPI_RW_Reg(WRITE_REG + STATUS,0x2E);	// 写一字节数据到寄存器
//
//		temp = SPI_Read(STATUS);	// 读寄存器一字节数据
//		SBUF = temp;	// 串口打印
//		while(!TI);
//		TI = 0;
//		DelayMs(100);
//		while(1);
	}
}