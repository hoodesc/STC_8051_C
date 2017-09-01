#include <stc12c2052ad.h>
#include <stdio.h>
#include "MYTYPE.h"
#include "Delay.h"
#include "NRF24L01_RX.h"

//NRF24L01 ֻ ���ͳ���


uchar const RX_ADDRESS[RX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// ���յ�ַ
uchar const TX_ADDRESS[TX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// ���͵�ַ

//	״̬λ
uchar bdata sta;
sbit RX_DR = sta^6;		// ���������ж�λ
sbit TX_DS = sta^5;		// ���������ж�λ
sbit MAX_RT = sta^4;	// �ط���������ж�λ
	
// SPIλ����
//sbit SCK  = P1^7;		// SPIʱ��
//sbit MISO = P1^6;		// ��������
//sbit MOSI = P1^5;		// ����ӳ�
//sbit CSN  = P1^4;		// оƬƬѡ��Ϊ��оƬ����
//sbit IRQ  = P1^3;		// �ж��ź�
//sbit CE   = P1^2;		// оƬģʽ������

//	SPIдһ�ֽ�
uchar SPI_RW(uchar dat)
{
	uchar i;
	for(i = 0; i < 8; i++)
	{
		MOSI = (dat&0x80);	// dat���λ����������
		dat = dat<<1;		// dat����1λ
		SCK	= 1;			// ����ʱ�ӣ���������ӻ��������ض��룩
		dat |= MISO;	  	// ���մӻ���������
		SCK = 0;			// ����ʱ��				 ���½��������
	}
	return(dat);			// ����NRF24L01״̬
}

//	SPIд�Ĵ���ֵ
uchar SPI_RW_Reg(uchar reg,uchar value)
{
	uchar status;
	CSN = 0;				// CSN�õͣ���ʼ��������
	status = SPI_RW(reg);	// ѡ��Ĵ�����ͬʱ����״̬��
	SPI_RW(value);			// д�����ݵ��Ĵ���
	CSN = 1;				// CSN�øߣ��������ݴ���
	return(status);
}

//	SPI��һ�ֽ�	
uchar SPI_Read(uchar reg)
{
	uchar reg_value;			// ��Ŷ����Ĵ�������

	CSN = 0;					// CSN�õͣ���ʼ��������
	SPI_RW(reg);				// ѡ��Ĵ���
	reg_value = SPI_RW(0);	// ��������
	CSN = 1;					// CSN�øߣ��������ݴ���
	return(reg_value);			// ����reg�Ĵ���ֵ		
}

// SPI��bytes�ֽ�����
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status,i;				 
	CSN = 0;					 // CSN�õͣ���ʼ��������
	status = SPI_RW(reg);		 // ѡ��Ĵ�����ͬʱ����״̬��
	for(i = 0; i < bytes; i++)
		pBuf[i] = SPI_RW(0);	 // �������ݴ�ŵ�����
	CSN = 1;					 // CSN�øߣ��������ݴ���
	return(status);
}  

//	SPIдbytes�ֽ�����
uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status,i;
	CSN = 0;					 // CSN�õͣ���ʼ��������
	status = SPI_RW(reg);		 // ѡ��Ĵ�����ͬʱ����״̬��
	for(i = 0; i < bytes; i++)
		SPI_RW(*pBuf++);		 // ����д���Ĵ���
	CSN = 1;					 // CSN�øߣ��������ݴ���
	return(status);
}

void NRF24L01_Init(void)
{
	CE  = 0;	// оƬʹ��	
	CSN = 1;	// SPIʧ��	
	SCK = 0;	// ʱ�ӳ�ʼ���͵�ƽ
	SPI_RW_Reg(WRITE_REG + EN_AA,0x00);			// ʧ���Զ�Ӧ��0~5
	SPI_RW_Reg(WRITE_REG + EN_RXADDR,0x01);		// ʹ�ܽ���ͨ��0
	SPI_RW_Reg(WRITE_REG + SETUP_RETR,0x00);	// ʧ���Զ��ط�
	SPI_RW_Reg(WRITE_REG + RF_CH,40);				// ���÷���Ƶ��2.44GHz
	SPI_RW_Reg(WRITE_REG + RF_SETUP,0x07);			// ���õ������Ŵ��������书��0dBm�����ݴ�����1Mbps
}

void SetRX_Mode()
{
	CE = 0;
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); 		//	����ͨ��0��ַ
	SPI_RW_Reg(WRITE_REG + RX_PW_P0,RX_PLOAD_WIDTH);								 	//	����ͨ��0 ���ݳ���
	SPI_RW_Reg(WRITE_REG + CONFIG,0x0F);	//	���ա��ϵ�ģʽ��2�ֽ�CRCУ�飬IRQ�շ�����ж�
	CE = 1;		//	����ģʽ
}

uchar NRF24L01_RxPacket(uchar *rx_buf)
{
	uchar re_flag = 0;
	sta = SPI_Read(STATUS);	//	��ȡ״̬�Ĵ���
	if(RX_DR)
	{
		LED = 0;
		CE = 0;
		SPI_Read_Buf(RD_TX_PLOAD,rx_buf,RX_PLOAD_WIDTH);
		re_flag = 1;
	}
	SPI_RW_Reg(WRITE_REG + STATUS,sta);		// ����ж�	
	return re_flag;
}