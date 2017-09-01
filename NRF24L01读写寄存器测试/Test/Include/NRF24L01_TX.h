#ifndef	__NRF24L01__TX_H__
#define __NRF24L01__TX_H__

extern uchar SPI_RW(uchar dat);
extern uchar SPI_Read(uchar reg);
extern uchar SPI_RW_Reg(uchar reg,uchar value);
extern uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar DataLen);
extern void NRF24L01_Init();
extern void nRF24L01_TXPacket();


// SPI位定义
sbit CE  = P1^7;		// 芯片使能
sbit CSN = P1^6;		// SPI控制位
sbit SCK = P1^5;		// SPI时钟
sbit MOSI  = P1^4;		// 主出从入
sbit MISO  = P1^3;		// 主入从出


// NRF24L01寄存器地址
#define CONFIG 		0x00		// 配置寄存器
#define EN_AA 		0x01		// 自动应答
#define EN_RXADDR 	0x02		// 接收地址允许
#define SETUP_AW 	0x03		// 设置地址宽度（所有数据通道）
#define SETUP_RETR 	0x04		// 自动重发
#define RF_CH 		0x05		// 射频通道
#define RF_SETUP 	0x06		// 射频寄存器
#define STATUS 		0x07		// 状态寄存器
#define OBSERVE_TX 	0x08		// 发送检测寄存器
#define CD 			0x09		// 载波检测
#define RX_ADDR_P0 0x0A		// 数据通道0~5接收地址
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR  0x10		// 发送地址
#define RX_PW_P0 0x11		// 数据通道0~5接收数据长度
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17		// FIFO状态寄存器

// NRF24L01操作指令
#define READ_REG 	0x00		// 读寄存器指令
#define WRITE_REG 	0x20		// 写寄存器指令
#define RD_TX_PLOAD 0x61		// 读取接收数据指令
#define WR_TX_PLOAD 0xA0		// 写待发送数据指令
#define FLUSE_TX 0xE1		 	// 清空TX FIFO寄存器
#define FLUSE_RX 0xE2			// 清空RX FIFO寄存器
#define REUSE_RX_PL 0xE3		// 应用发射，重新发送上一包有效数据
#define NOP 0xFF				// 空操作 可用于读寄存器状态

#define RX_ADR_WITH 5		// 收地址5个字节宽度
#define TX_ADR_WITH 5		// 发地址5个字节宽度
#define RX_PLOAD_WITH 4	// 收数据32个字节
#define TX_PLOAD_WITH 4	// 发数据32个字节
#endif