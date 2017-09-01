#include <stc12c2052ad.h>
#include <stdio.h>
#include "MYTYPE.h"
#include "Delay.h"
#include "NRF24L01_TX.h"

//NRF24L01 ֻ ���ͳ���


uchar const RX_ADDRESS[RX_ADR_WITH] = {0x12,0x21,0xaa,0x34,0x43};		// ���յ�ַ
uchar const TX_ADDRESS[TX_ADR_WITH] = {0x12,0x21,0xaa,0x34,0x43};		// ���͵�ַ

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
	reg_value = SPI_RW(0x00);	// ��������
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
		pBuf[i] = SPI_RW(0x00);	 // �������ݴ�ŵ�����
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

