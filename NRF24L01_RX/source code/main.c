#include <stc12c2052ad.h>
#include "MYTYPE.h"
#include "NRF24L01_RX.h"
#include "Delay.h"
#include "keyscan.h"

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
/**********************************/
//	串口调试
void Uart_print(void)
{
	uchar temp = 0;
	temp = SPI_Read(CD);
	SBUF = temp;
	while(!TI);
	TI = 0;
}


void main()
{	
	Uart_Init();		//	串口初始化
	NRF24L01_Init();	//	NRF24L01初始化
	DelayMs(1000);
	while(1)
	{
//		SetRX_Mode();
//		NRF24L01_RxPacket(RX_DAT);
		Uart_print();
//		motor();
		
	}
}