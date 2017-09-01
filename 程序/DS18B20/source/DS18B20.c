#include <stc15.h>
#include "MYTYPE.h"
#include "DS18B20.h"
#include "delay.h"
#include "nrf24l01_tx.h"


uchar code number[] = {'0','1','2','3','4','5','6','7','8','9'};
uchar c[] = {'0','0','.','0',0xA1,0xE6,'\0'};
//uchar c[] = {'0','0','.','0',0x00,0xB0,'\0'};
sbit DQ = P3^3;
bit DS18B20_flag = 0;

void DS18B20_Write(uchar udata)
{
	uchar i;
	uchar dat;
	for(i = 0; i < 8; i++)
	{
		dat = udata & 0x01;
		udata = udata >> 1;		// udata����һλ׼����һ����������
		if(dat)
		{
			DQ = 0;		// ��������
			DelayUs(1);	// ��ʱ2us
			DQ = 1;		// ��1
			DelayUs(40);	// ��ʱ80us
			DQ = 1;		// �ͷ�����
			DelayUs(1);	// ��ʱ2us
		}
		else
		{
			DQ = 0;		// ��������
			DelayUs(1);	// ��ʱ2us
			DQ = 0;		// ��0
			DelayUs(40);	// ��ʱ80us;
			DQ = 1;		// �ͷ�����
			DelayUs(1);	// ��ʱ2us;
		}
	}	
}

uchar DS18B20_Read()
{
	uchar i;
	uchar udata = 0x00;
	for(i = 0; i < 8; i++)
	{
		udata = udata >> 1;	// udata����һλ����һ����λΪ�ղ���
		DQ = 0;			// ��������
		DelayUs(2);		// ��ʱ4us��������ʱ϶
		DQ = 1;			// ��������
		if(DQ)
			udata |= 0x80;		// 1����DQ���ݲ������udata���λ
		else
			udata |= 0x00;		// 0����DQ���ݲ������udata���λ
		DelayUs(40);		// ��ʱ80us
		DQ = 1;			// �ͷ�����
		DelayUs(1);		// ��ʱ2us
	}
	return(udata);	
}

void DS18B20_Init()
{
	DQ = 1;			// ��������
	DelayUs(1);
	
	DQ = 0;			// ��������
	DelayUs(350);		// ��ʱ700us	
	DQ = 1;			// �ͷ�����
	DelayUs(15);		// ��ʱ30us
	while(DQ);		// �ȴ�DS18B20��������
		//LED = 0;	// ��λ�ɹ�������LED
	DelayUs(100);		// ��ʱ200us
	DQ = 1;			// �ͷ�����
	DelayUs(150);		// ��ʱ300us
}

void compute(uchar lsb,uchar msb)
{
	uchar ge,shi,bai;
	uint num;
	uint temp;
	num = msb;
	num  <<= 8;
	num |= lsb;
	temp = num*0.0625*10+0.5;	
	bai = temp/100;  	
	shi = temp%100/10;	
	ge  = temp%100%10;	
	c[0] = number[bai];
	c[1] = number[shi];
	c[3] = number[ge];
}
void DS18B20_Temperature_Read()
{
	uchar i;
	static uchar k = 0;
	// ��ʼ���������¶�ת��
	DS18B20_Init();			// DS18B20��ʼ��
	DS18B20_Write(0xCC);	// ����ROM��ַ
	DS18B20_Write(0x44);	// �����¶�ת��
	if(!k)
	{
		k = 1;
		DelayMs(800);
	}
	else if(k)
	{
		for(i = 0; i < 4; i++)
		{
			SPI_RW_Reg(WRITE_REG + STATUS,0x7E);				//	���״̬�Ĵ���
			SPI_Write_Buf(W_TX_PAYLOAD,TX_DAT,TX_PLOAD_WIDTH);	//	װ������
			NRF24L01_TxPacket(TX_DAT);		//	����
			DelayMs(50);
		}	
	}
	// ��ʼ�������¶�
	DS18B20_Init();			// DS18B20��ʼ��
	DS18B20_Write(0xCC);	// ����ROM��ַ
	DS18B20_Write(0xBE);	// ��ȡRAM����
	compute(DS18B20_Read(),DS18B20_Read());		// 	�ȶ���8λ���ٶ��߰�λ
	TX_DAT[1] = c[0];
	TX_DAT[2] = c[1];	
	TX_DAT[3] = c[3];	
//	for(i = 0; i < 6; i++)	// �������
//	{
//		SBUF = c[i];
//		while(!TI);
//		TI = 0;
//	}
}