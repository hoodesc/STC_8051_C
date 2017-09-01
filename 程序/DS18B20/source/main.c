#include <STC15.h> 
#include "MYTYPE.h"
#include "Delay.h"
#include "UART.h"
#include "nrf24l01_tx.h"
#include "ds18b20.h"

//	温度传感器
//	NRF24L01 频道50
//	收发地址：0x12,0x21,0xaa,0x34,0x43
//	设备码A0


//void Timer0_Init()
//{
//	AUXR &= 0x7F;
//	TMOD &= 0xF0;
//	TL0 = 0x00;	
//	TH0 = 0xDC;	
//	ET0 = 0;
//	EA  = 1;
//	TR0 = 1;		
//}

//void Timer0_ISR()	interrupt 1
//{
//		static uchar time = 0;
//		time++;
//		if(time > 10)
//		{
//			SPI_RW_Reg(WRITE_REG + STATUS,0x7E);				//	清除状态寄存器
//			SPI_Write_Buf(W_TX_PAYLOAD,TX_DAT,TX_PLOAD_WIDTH);	//	装载数据
//			NRF24L01_TxPacket(TX_DAT);		//	发送		
//		}
//}

void main(void)
{
//	uchar i = 0;
	P1M0 = 0x00;
	P1M1 = 0x00;
	DS18B20_Init();
//	Timer0_Init();
	Uart_Init();
	SetTX_Mode();
	DelayMs(500);
	while(1)
	{
		DS18B20_Temperature_Read();
//		SPI_RW_Reg(WRITE_REG + STATUS,0x7E);				//	清除状态寄存器
//		SPI_Write_Buf(W_TX_PAYLOAD,TX_DAT,TX_PLOAD_WIDTH);	//	装载数据
//		NRF24L01_TxPacket(TX_DAT);		//	发送	
	}		
}