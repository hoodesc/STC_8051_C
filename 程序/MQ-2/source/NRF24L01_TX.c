#include <STC15.h>
#include "MYTYPE.h"
#include "NRF24L01_TX.h"
#include "delay.h"

uchar const RX_ADDRESS[RX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// 接收地址
uchar const TX_ADDRESS[TX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// 发送地址
uchar TX_DAT[TX_PLOAD_WIDTH] = {0xA1,0xAA,0x00,0x00,0x00};		//	发送内容


//	NRF24L01 脚管定义
//sbit CE  = P1^7;		// 芯片使能
//sbit CSN = P1^6;		// SPI控制位
//sbit SCK = P1^5;		// SPI时钟
//sbit MOSI  = P1^4;		// 主出从入
//sbit MISO  = P1^3;		// 主入从出

sbit CE  = P1^2;		// 芯片使能
sbit CSN = P1^3;		// SPI控制位
sbit SCK = P1^4;		// SPI时钟
sbit MOSI  = P1^5;		// 主出从入
sbit MISO  = P1^6;		// 主入从出

//	SPI写一字节
uchar SPI_RW(uchar dat)
{
	uchar i;
	for(i = 0; i < 8; i++)
	{
		MOSI = (dat & 0x80);	//	最高位放到线上
		dat  = (dat << 1);		//	数据左移一位
		SCK  = 1;				//	上升沿读入
		dat |= MISO;			//	接收从机返回数据
		SCK	 = 0;				//	下降沿输出
	}
	return(dat);
}

//	SPI写寄存器值
uchar SPI_RW_Reg(uchar reg,uchar value)
{
	uchar status;
	CSN = 0;				//	使能SPI，开始数据传输
	status = SPI_RW(reg);	//	选择寄存器，同时返回状态字
	SPI_RW(value);			//	写值到reg
	CSN = 1;				//	失能SPI，结束数据传输
	return(status);
}

//	SPI读一字节
uchar SPI_Read(uchar reg)
{
	uchar reg_value;	//	存放寄存器值
	CSN = 0;			//	使能SPI，开始数据传输
	SPI_RW(reg);
	reg_value = SPI_RW(0);	//	读取reg的值到reg_value
	CSN = 1;			//	失能SPI，结束数据传输
	return(reg_value);						
}

//	SPI读bytes字节
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status,i;
	CSN = 0;						//	使能SPI，开始数据传输
	status = SPI_RW(reg);			//	选择寄存器	
	for(i = 0; i < bytes; i++)
		pBuf[i] = SPI_RW(0);		//	读出数据
	CSN = 1;		    			//	失能SPI，结束数据传输
	return(status);
}

//	SPI写bytes字节
uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status,i;
	CSN = 0;		   			//	使能SPI，开始数据传输
	status = SPI_RW(reg);		//	选择寄存器
	for(i = 0; i < bytes; i++)
		SPI_RW(*pBuf++);		//	写入bytes字节数据
	CSN = 1;		   			//	失能SPI，结束数据传输
	return(status);
}

//	NRF24L01发送函数
void NRF24L01_TxPacket(uchar *pTxBuf)
{
	CE = 0;
	SPI_Write_Buf(WRITE_REG + TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);			//	写本地地址
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);		//	写接收端地址	
	SPI_Write_Buf(W_TX_PAYLOAD,pTxBuf,TX_PLOAD_WIDTH);			//	装载发送内容
	SPI_RW_Reg(WRITE_REG + CONFIG,0x0E);						//	CRC使能、2字节CRC校验值、上电、发射模式	
	CE = 1;
	DelayMs(1);	//	延时10us以上后发射	
}

//	NRF24L01初始化发射模式
void SetTX_Mode(void)
{
	CE  = 0;	//	芯片使能
	CSN = 1;	//	失能SPI	
	SCK = 0;	//	时钟线初始化为低电平
	SPI_RW_Reg(WRITE_REG + EN_AA,0x00);			//	关闭数据通道0~5自动应答
	SPI_RW_Reg(WRITE_REG + SETUP_RETR,0x00);	//	关闭自动重发
	SPI_RW_Reg(WRITE_REG + EN_RXADDR,0x01);		//	关闭接收通道0~5
	SPI_RW_Reg(WRITE_REG + RX_PW_P0,RX_PLOAD_WIDTH);	//	接收通道0接收数据长度 5字节
	SPI_RW_Reg(WRITE_REG + RF_CH,50);			//	射频通道2.44GHz
	SPI_RW_Reg(WRITE_REG + RF_SETUP,0x07);		//	低噪声放大器增益、0dBm、2Mbps
}