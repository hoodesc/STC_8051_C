#include <stc12c2052ad.h>
#include "MYTYPE.h"
#include "NRF24L01_RX.h"
#include "Delay.h"
#include "main.h"
#include "keyscan.h"
//NRF24L01 ֻ ���ճ���


uchar const RX_ADDRESS[RX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// ���յ�ַ
uchar const TX_ADDRESS[TX_ADR_WIDTH] = {0x12,0x21,0xaa,0x34,0x43};		// ���͵�ַ

uchar RX_DAT[RX_PLOAD_WIDTH] = {0};	//	������������

uchar bdata sta;
sbit RX_DR  = sta^6;
sbit TX_DR  = sta^5;
sbit MAX_RT = sta^4;

	
//	NRF24L01 �Źܶ���
sbit CE  = P1^7;		// оƬʹ��
sbit CSN = P1^6;		// SPI����λ
sbit SCK = P1^5;		// SPIʱ��
sbit MOSI  = P1^4;		// ��������
sbit MISO  = P1^3;		// ����ӳ�

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
		SCK  = 0;				//	�½������		
	}
	return(dat);
}

//	SPIд�Ĵ���ֵ
uchar SPI_RW_Reg(uchar reg,uchar value)
{
	uchar status;
	CSN = 0;				//	SPIʹ�ܣ���ʼ��������
	status = SPI_RW(reg);	//	ѡ��Ĵ���
	SPI_RW(value);			//	дֵ���Ĵ���
	CSN = 1;				//	SPIʧ�ܣ��������ݴ���
	return(status);
}

//	SPI���Ĵ���ֵ
uchar SPI_Read(uchar reg)
{
	uchar reg_value;
	CSN = 0;					//	SPIʹ�ܣ���ʼ��������
	SPI_RW(reg);				//	ѡ��Ĵ���
	reg_value = SPI_RW(0); 		//	��ȡ�Ĵ���ֵ��reg_value
	CSN = 1;					//	SPIʧ�ܣ��������ݴ���
	return(reg_value);
}

//	SPIдbytes�ֽ�
uchar SPI_Write_Buf(uchar reg,uchar *pBuf,uchar bytes)
{	
	uchar status;
	uchar i;
	CSN = 0;					//	SPIʹ�ܣ���ʼ��������
	status = SPI_RW(reg);		//	ѡ��Ĵ���
	for(i = 0; i < bytes; i++)
		SPI_RW(*pBuf++);		//	дbytes�ֽ�ֵ���Ĵ���
	CSN = 1;					//	SPIʧ�ܣ��������ݴ���
	return(status);
}

//	SPI��bytes�ֽ�
uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar bytes)
{
	uchar status;
	uchar i;
	CSN = 0;					 //	SPIʹ�ܣ���ʼ��������
	status = SPI_RW(reg);		 //	ѡ��Ĵ���
	for(i = 0; i < bytes; i++)
		pBuf[i] = SPI_RW(0);	 //	���Ĵ���bytes�ֽ�ֵ	
	CSN = 1;					 //	SPIʧ�ܣ��������ݴ���
	return(status);
}

//	NRF24L01����ģʽ��ʼ��
void NRF24L01_Init()
{
	CE = 0;		//	оƬʹ��
	CSN = 1;	//	ʧ��SPI
	SCK = 0;	//	SPIʱ���߳�ʼ�͵�ƽ
	SPI_Write_Buf(WRITE_REG + TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);		//	д���ص�ַ
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);	//	д���ն˵�ַ
	SPI_RW_Reg(WRITE_REG + RX_PW_P0,RX_PLOAD_WIDTH);				//	����ͨ��0���ݳ���
	SPI_RW_Reg(WRITE_REG + EN_AA,0x00);			//	�ر�����ͨ��0~5�Զ�Ӧ��
	SPI_RW_Reg(WRITE_REG + SETUP_RETR,0x00);	//	�ر��Զ��ط�
	SPI_RW_Reg(WRITE_REG + EN_RXADDR,0x01);		//	�򿪽���ͨ��0
	SPI_RW_Reg(WRITE_REG + RF_CH,40);	//	��Ƶͨ��2.44GHz
	SPI_RW_Reg(WRITE_REG + RF_SETUP,0x07);	//	�������Ŵ���������0dBm��2Mbps
}

void SetRX_Mode(void)
{
	CE = 0;
	SPI_RW_Reg(WRITE_REG + CONFIG,0x0F);				//	���ա��ϵ�ģʽ��2�ֽ�CRCУ�顢IRQ�շ�����ж�	
	CE = 1;
	DelayUs(200);
	//	����ģʽ
}

//	NRF24L01���պ���
void NRF24L01_RxPacket(uchar *pBuf)
{
	uchar i;
	sta = SPI_Read(STATUS);		//	��״̬�Ĵ���
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
		SPI_RW_Reg(WRITE_REG + STATUS,0x7E);		//	����ж�	
	}
	
}