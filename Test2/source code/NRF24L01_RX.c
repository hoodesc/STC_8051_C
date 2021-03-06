#include <stc12c2052ad.h>
#include <stdio.h>
#include "MYTYPE.h"
#include "Delay.h"
#include "NRF24L01_RX.h"

//NRF24L01 只 发送程序


uchar const RX_ADDRESS[RX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// 接收地址
uchar const TX_ADDRESS[TX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// 发送地址

//	状态位
uchar bdata sta;
sbit RX_DR = sta^6;		// 接收数据中断位
sbit TX_DS = sta^5;		// 发送数据中断位
sbit MAX_RT = sta^4;	// 重发次数溢出中断位
	
// SPI位定义
//sbit SCK  = P1^7;		// SPI时钟
//sbit MISO = P1^6;		// 主出从入
//sbit MOSI = P1^5;		// 主入从出
//sbit CSN  = P1^4;		// 芯片片选，为低芯片工作
//sbit IRQ  = P1^3;		// 中断信号
//sbit CE   = P1^2;		// 芯片模式控制线

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
	reg_value = SPI_RW(0);	// 读出数据
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
		pBuf[i] = SPI_RW(0);	 // 读出数据存放到数组
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

void NRF24L01_Init(void)
{
	CE  = 0;	// 芯片使能	
	CSN = 1;	// SPI失能	
	SCK = 0;	// 时钟初始化低电平
	SPI_RW_Reg(WRITE_REG + EN_AA,0x00);			// 失能自动应答0~5
	SPI_RW_Reg(WRITE_REG + EN_RXADDR,0x01);		// 使能接收通道0
	SPI_RW_Reg(WRITE_REG + SETUP_RETR,0x00);	// 失能自动重发
	SPI_RW_Reg(WRITE_REG + RF_CH,40);				// 设置发射频率2.44GHz
	SPI_RW_Reg(WRITE_REG + RF_SETUP,0x07);			// 设置低噪声放大器，发射功率0dBm，数据传输率1Mbps
}

void SetRX_Mode()
{
	CE = 0;
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 		//	接收通道0地址
	SPI_RW_Reg(WRITE_REG + RX_PW_P0,RX_PLOAD_WIDTH);								 	//	接收通道0 数据长度
	SPI_RW_Reg(WRITE_REG + CONFIG,0x0F);	//	接收、上电模式、2字节CRC校验，IRQ收发完成中断
	CE = 1;		//	接收模式
}

uchar NRF24L01_RxPacket(uchar *rx_buf)
{
	uchar re_flag = 0;
	sta = SPI_Read(STATUS);	//	读取状态寄存器
	if(RX_DR)
	{
		LED = 0;
		CE = 0;
		SPI_Read_Buf(RD_TX_PLOAD,rx_buf,RX_PLOAD_WIDTH);
		re_flag = 1;
	}
	SPI_RW_Reg(WRITE_REG + STATUS,sta);		// 清除中断	
	return re_flag;
}