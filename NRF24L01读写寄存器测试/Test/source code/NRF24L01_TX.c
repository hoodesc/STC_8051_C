#include <stc12c2052ad.h>
#include <stdio.h>
#include "MYTYPE.h"
#include "Delay.h"
#include "NRF24L01_TX.h"

//NRF24L01 只 发送程序


uchar const RX_ADDRESS[RX_ADR_WITH] = {0x12,0x21,0xaa,0x34,0x43};		// 接收地址
uchar const TX_ADDRESS[TX_ADR_WITH] = {0x12,0x21,0xaa,0x34,0x43};		// 发送地址

//	SPI写一字节
uchar SPI_RW(uchar dat)
{
	uchar i;
	for(i = 0; i < 8; i++)
	{
		MOSI = (dat&0x80);	// dat最高位放入数据线
		dat = dat<<1;		// dat左移1位
		SCK	= 1;			// 拉高时钟，数据送入从机（上升沿读入）
		dat |= MISO;	  	// 接收从机传来数据
		SCK = 0;			// 拉低时钟				 （下降沿输出）
	}
	return(dat);			// 返回NRF24L01状态
}

//	SPI写寄存器值
uchar SPI_RW_Reg(uchar reg,uchar value)
{
	uchar status;
	CSN = 0;				// CSN置低，开始传输数据
	status = SPI_RW(reg);	// 选择寄存器，同时返回状态字
	SPI_RW(value);			// 写入数据到寄存器
	CSN = 1;				// CSN置高，结束数据传输
	return(status);
}

//	SPI读一字节	
uchar SPI_Read(uchar reg)
{
	uchar reg_value;			// 存放读出寄存器数据

	CSN = 0;					// CSN置低，开始传输数据
	SPI_RW(reg);				// 选择寄存器
	reg_value = SPI_RW(0x00);	// 读出数据
	CSN = 1;					// CSN置高，结束数据传输
	return(reg_value);			// 返回reg寄存器值		
}

// SPI读bytes字节数据
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status,i;				 
	CSN = 0;					 // CSN置低，开始传输数据
	status = SPI_RW(reg);		 // 选择寄存器，同时返回状态字
	for(i = 0; i < bytes; i++)
		pBuf[i] = SPI_RW(0x00);	 // 读出数据存放到数组
	CSN = 1;					 // CSN置高，结束数据传输
	return(status);
}

//	SPI写bytes字节数据
uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status,i;
	CSN = 0;					 // CSN置低，开始传输数据
	status = SPI_RW(reg);		 // 选择寄存器，同时返回状态字
	for(i = 0; i < bytes; i++)
		SPI_RW(*pBuf++);		 // 数据写进寄存器
	CSN = 1;					 // CSN置高，结束数据传输
	return(status);
}

