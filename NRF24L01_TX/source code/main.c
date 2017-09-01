#include <STC15.h>
#include "MYTYPE.h"
#include "Delay.h"
#include "NRF24L01_TX.h"
#include "main.h"
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
//void Uart_print(void)
//{
//	uchar temp = 0;
//	temp = SPI_Read(0x00);
//	SBUF = temp;
//	while(!TI);
//	TI = 0;
//}

void Timer0_Init(void)
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	TR0 = 1;
}

void main()
{
//	Uart_Init();
	P3M1 = 0x00;
	P3M0 = 0x00;
	Timer0_Init();
	SetTX_Mode();
	DelayMs(500);
	while(1)
	{
		if(TF0)
		{
			TF0 = 0;
			TL0 = 0x00;		//设置定时初值
			TH0 = 0xDC;		//设置定时初值
			Keyscan();	
		}

//	SPI_RW_Reg(WRITE_REG + STATUS,0x7E);				//	清除状态寄存器
//	SPI_Write_Buf(W_TX_PAYLOAD,TX_DAT,TX_PLOAD_WIDTH);	//	装载数据
//	NRF24L01_TxPacket(TX_DAT);		//	发送
//	while(1);
//	//	发送数据后打印status、fifo_status寄存器
//	while(1);
	}
}