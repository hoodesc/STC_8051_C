#include <stc12c2052ad.h>
#include "MYTYPE.h"
#include "NRF24L01_RX.h"
#include "Delay.h"
#include "main.h"
#include "keyscan.h"
//NRF24L01 只 接收程序


uchar const RX_ADDRESS[RX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// 接收地址
uchar const TX_ADDRESS[TX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// 发送地址

uchar RX_DAT[RX_PLOAD_WIDTH] = {0};	//	接收内容数组

uchar bdata sta;
sbit RX_DR  = sta^6;
sbit TX_DR  = sta^5;
sbit MAX_RT = sta^4;

	
//	NRF24L01 脚管定义
sbit CE  = P1^7;		// 芯片使能
sbit CSN = P1^6;		// SPI控制位
sbit SCK = P1^5;		// SPI时钟
sbit MOSI  = P1^4;		// 主出从入
sbit MISO  = P1^3;		// 主入从出

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
		SCK  = 0;				//	下降沿输出		
	}
	return(dat);
}

//	SPI写寄存器值
uchar SPI_RW_Reg(uchar reg,uchar value)
{
	uchar status;
	CSN = 0;				//	SPI使能，开始传输数据
	status = SPI_RW(reg);	//	选择寄存器
	SPI_RW(value);			//	写值到寄存器
	CSN = 1;				//	SPI失能，结束数据传输
	return(status);
}

//	SPI读寄存器值
uchar SPI_Read(uchar reg)
{
	uchar reg_value;
	CSN = 0;					//	SPI使能，开始传输数据
	SPI_RW(reg);				//	选择寄存器
	reg_value = SPI_RW(0); 		//	读取寄存器值到reg_value
	CSN = 1;					//	SPI失能，结束数据传输
	return(reg_value);
}

//	SPI写bytes字节
uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar bytes)
{	
	uchar status;
	uchar i;
	CSN = 0;					//	SPI使能，开始传输数据
	status = SPI_RW(reg);		//	选择寄存器
	for(i = 0; i < bytes; i++)
		SPI_RW(*pBuf++);		//	写bytes字节值到寄存器
	CSN = 1;					//	SPI失能，结束数据传输
	return(status);
}

//	SPI读bytes字节
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status;
	uchar i;
	CSN = 0;					 //	SPI使能，开始传输数据
	status = SPI_RW(reg);		 //	选择寄存器
	for(i = 0; i < bytes; i++)
		pBuf[i] = SPI_RW(0);	 //	读寄存器bytes字节值	
	CSN = 1;					 //	SPI失能，结束数据传输
	return(status);
}

//	NRF24L01接收模式初始化
void NRF24L01_Init()
{
	CE = 0;		//	芯片使能
	CSN = 1;	//	失能SPI
	SCK = 0;	//	SPI时钟线初始低电平
	SPI_Write_Buf(WRITE_REG + TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);		//	写本地地址
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);	//	写接收端地址
	SPI_RW_Reg(WRITE_REG + RX_PW_P0,RX_PLOAD_WIDTH);				//	接收通道0数据长度
	SPI_RW_Reg(WRITE_REG + EN_AA,0x00);			//	关闭数据通道0~5自动应答
	SPI_RW_Reg(WRITE_REG + SETUP_RETR,0x00);	//	关闭自动重发
	SPI_RW_Reg(WRITE_REG + EN_RXADDR,0x01);		//	打开接收通道0
	SPI_RW_Reg(WRITE_REG + RF_CH,40);	//	射频通道2.44GHz
	SPI_RW_Reg(WRITE_REG + RF_SETUP,0x07);	//	低噪声放大增益器、0dBm、2Mbps
}

void SetRX_Mode(void)
{
	CE = 0;
	SPI_RW_Reg(WRITE_REG + CONFIG,0x0F);				//	接收、上电模式、2字节CRC校验、IRQ收发完成中断	
	CE = 1;
	DelayUs(200);
	//	接收模式
}

//	NRF24L01接收函数
void NRF24L01_RxPacket(uchar *pBuf)
{
	uchar i;
	sta = SPI_Read(STATUS);		//	读状态寄存器
	if(RX_DR)
	{
		CE = 0;
		SPI_Read_Buf(R_TX_PAYLOAD,pBuf,RX_PLOAD_WIDTH);

		if(RX_DAT[0] == 0xaa && RX_DAT[1] == 0xbb && RX_DAT[2] == 0xcc && RX_DAT[3] == 0xDD && RX_DAT[4] == 0xEE)
			motor();
		for(i = 0; i < 5; i++)
		{
			SBUF = RX_DAT[i];
			RX_DAT[i] = 0x00;
			while(!TI);
			TI = 0;				
		}
		SPI_RW_Reg(WRITE_REG + STATUS,0x7E);		//	清除中断	
	}
	
}