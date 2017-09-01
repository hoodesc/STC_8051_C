#include <STC15.h>
#include "MYTYPE.h"
#include "NRF24L01_TX.h"
#include "delay.h"

uchar const RX_ADDRESS[RX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// ���յ�ַ
uchar const TX_ADDRESS[TX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// ���͵�ַ
uchar TX_DAT[TX_PLOAD_WIDTH] = {0xA2,0xAA,0x00,0x00,0x00};		//	��������

//	NRF24L01 �Źܶ���
//sbit CE  = P1^7;		// оƬʹ��
//sbit CSN = P1^6;		// SPI����λ
//sbit SCK = P1^5;		// SPIʱ��
//sbit MOSI  = P1^4;		// ��������
//sbit MISO  = P1^3;		// ����ӳ�

sbit CE  = P1^2;		// оƬʹ��
sbit CSN = P1^3;		// SPI����λ
sbit SCK = P1^4;		// SPIʱ��
sbit MOSI  = P1^5;		// ��������
sbit MISO  = P1^6;		// ����ӳ�

//	SPIдһ�ֽ�
uchar SPI_RW(uchar dat)
{
	uchar i;
	for(i = 0; i < 8; i++)
	{
		MOSI = (dat & 0x80);	//	���λ�ŵ�����
		dat  = (dat << 1);		//	��������һλ
		SCK  = 1;				//	�����ض���
		dat |= MISO;			//	���մӻ���������
		SCK	 = 0;				//	�½������
	}
	return(dat);
}

//	SPIд�Ĵ���ֵ
uchar SPI_RW_Reg(uchar reg,uchar value)
{
	uchar status;
	CSN = 0;				//	ʹ��SPI����ʼ���ݴ���
	status = SPI_RW(reg);	//	ѡ��Ĵ�����ͬʱ����״̬��
	SPI_RW(value);			//	дֵ��reg
	CSN = 1;				//	ʧ��SPI���������ݴ���
	return(status);
}

//	SPI��һ�ֽ�
uchar SPI_Read(uchar reg)
{
	uchar reg_value;	//	��żĴ���ֵ
	CSN = 0;			//	ʹ��SPI����ʼ���ݴ���
	SPI_RW(reg);
	reg_value = SPI_RW(0);	//	��ȡreg��ֵ��reg_value
	CSN = 1;			//	ʧ��SPI���������ݴ���
	return(reg_value);						
}

//	SPI��bytes�ֽ�
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status,i;
	CSN = 0;						//	ʹ��SPI����ʼ���ݴ���
	status = SPI_RW(reg);			//	ѡ��Ĵ���	
	for(i = 0; i < bytes; i++)
		pBuf[i] = SPI_RW(0);		//	��������
	CSN = 1;		    			//	ʧ��SPI���������ݴ���
	return(status);
}

//	SPIдbytes�ֽ�
uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status,i;
	CSN = 0;		   			//	ʹ��SPI����ʼ���ݴ���
	status = SPI_RW(reg);		//	ѡ��Ĵ���
	for(i = 0; i < bytes; i++)
		SPI_RW(*pBuf++);		//	д��bytes�ֽ�����
	CSN = 1;		   			//	ʧ��SPI���������ݴ���
	return(status);
}

//	NRF24L01���ͺ���
void NRF24L01_TxPacket(uchar *pTxBuf)
{
	CE = 0;
	SPI_Write_Buf(WRITE_REG + TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);			//	д���ص�ַ
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);		//	д���ն˵�ַ	
	SPI_Write_Buf(W_TX_PAYLOAD,pTxBuf,TX_PLOAD_WIDTH);			//	װ�ط�������
	SPI_RW_Reg(WRITE_REG + CONFIG,0x0E);						//	CRCʹ�ܡ�2�ֽ�CRCУ��ֵ���ϵ硢����ģʽ	
	CE = 1;
	DelayMs(1);	//	��ʱ10us���Ϻ���	
}

//	NRF24L01��ʼ������ģʽ
void SetTX_Mode(void)
{
	CE  = 0;	//	оƬʹ��
	CSN = 1;	//	ʧ��SPI	
	SCK = 0;	//	ʱ���߳�ʼ��Ϊ�͵�ƽ
	SPI_RW_Reg(WRITE_REG + EN_AA,0x00);			//	�ر�����ͨ��0~5�Զ�Ӧ��
	SPI_RW_Reg(WRITE_REG + SETUP_RETR,0x00);	//	�ر��Զ��ط�
	SPI_RW_Reg(WRITE_REG + EN_RXADDR,0x01);		//	�رս���ͨ��0~5
	SPI_RW_Reg(WRITE_REG + RX_PW_P0,RX_PLOAD_WIDTH);	//	����ͨ��0�������ݳ��� 5�ֽ�
	SPI_RW_Reg(WRITE_REG + RF_CH,50);			//	��Ƶͨ��2.44GHz
	SPI_RW_Reg(WRITE_REG + RF_SETUP,0x07);		//	�������Ŵ������桢0dBm��2Mbps
}