#include <stc15.h>
#include <intrins.h>
#include "mytype.h"
#include "ds1302.h"
#include "delay.h"
#include "lcd12864.h"

sbit DS_SCLK = P2^1;
sbit DS_IO = P2^2;
sbit DS_CE = P2^3;

uchar DATE[] = {20,42,00,16,11,29};	// ʱ �� �� �� �� ��
uchar code DATE_Write_Add[]= {0x84,0x82,0x80,0x8C,0x88,0x86};		// д�Ĵ���
uchar code DATE_Read_Add[] = {0x85,0x83,0x81,0x8D,0x89,0x87};		// ���Ĵ���
uchar code WEI[] = {0xFE,0xFD,0xFB,0xF7,0xFF};		// �����λ��
//uchar code DUAN[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};	// ���������0~F ������
uchar code ASCII[] = {'0','1','2','3','4','5','6','7','8','9'};
uchar DATE_Buf[6] = {0x00};
uchar Dis_DATE[12] = {0x00};

void Write_DS1302_Byte(uchar dat)
{
	uchar i;
	for(i = 0; i < 8; i++)
	{
	 	DS_SCLK = 0;
		DS_IO = dat & 0x01;
		DS_SCLK = 1;
		dat >>= 1;
	}
}

void Write_DS1302(uchar add,uchar dat)
{
	DS_CE = 0;
	DelayUs(1);
	DS_SCLK = 0;
	DelayUs(1);
	DS_CE = 1;		// ʹ��оƬ
	DelayUs(1);
	Write_DS1302_Byte(add);
	Write_DS1302_Byte(dat);
	DS_CE = 0;		// ʧ��оƬ
	DelayUs(1);
	DS_IO = 1;		// �ͷ�����
	DS_SCLK = 1;	// �ͷ�ʱ��
}

uchar Read_DS1302_Byte(uchar add)
{
	uchar temp = 0x00;
	uchar i;
	DS_CE = 0;
	DelayUs(1);
	DS_SCLK = 0;
	DelayUs(1);
	DS_CE = 1;		// ʹ��оƬ
	DelayUs(1);
	Write_DS1302_Byte(add);
	for(i = 0; i < 8; i++)
	{	
		temp >>= 1;	
		DS_SCLK = 0;		
		if(DS_IO)	 
			temp = temp|0x80;
		DS_SCLK = 1;		 
	}	
	DS_CE = 0;		// ʧ��оƬ
	DelayUs(1);
	DS_IO = 1;		// �ͷ�����
	DS_SCLK = 1;	// �ͷ�ʱ��
	return temp;	
}

void Set_DS1302_TIME() 
{
	uchar i,ge,shi;
	for(i = 0; i < 6; i++)
	{
		shi = DATE[i]/10;
		ge  = DATE[i]%10;
		DATE[i] = shi<<4 | ge;	
	}
	for(i = 0; i < 6; i++)
	{
		Write_DS1302(DATE_Write_Add[i],DATE[i]);
		DelayUs(200);
	}
}

void DS1302_Init()
{
		Write_DS1302(0x8E,0x00);	// ȥ��д����
			DelayUs(200);
		Set_DS1302_TIME();				// д��ʱ������
		Write_DS1302(0x8E,0x80);	// ��д����
}

void DS1302_Time_Display(void)
{
	uchar i;
	for(i = 0; i < 6; i++)
	{		
		DATE_Buf[i] = Read_DS1302_Byte(DATE_Read_Add[i]);
		DelayUs(100);
		Dis_DATE[i*2] = DATE_Buf[i] >> 4;
		Dis_DATE[i*2+1] = DATE_Buf[i] & 0x0F;									
	}
	LCD_Write_com(0x80+0x1C); // ����
	DelayUs(100);			
	for(i = 0; i < 6; i++)     
	{
//		SBUF = Dis_DATE[i]+0x30;
//		while(!TI);
//		TI = 0;	
		if(i == 2 || i == 4)
			LCD_Write_data(':');					
		LCD_Write_data(Dis_DATE[i]+0x30);  
		DelayUs(100);
	}
}

//void main(void)
//{
//	uchar i;
//	Uart_Init();				// ���ڳ�ʼ��				
//	Write_DS1302(0x8E,0x00);	// ȥ��д����
//	//Set_DS1302_TIME();			// д��ʱ������
//	Write_DS1302(0x8E,0x80);	// ��д����
//	while(1)
//	{
//		for(i = 0; i < 6; i++)
//		{
//			DATE_Buf[i] = Read_DS1302_Byte(DATE_Read_Add[i]);
//			Dis_DATE[i*2] = DATE_Buf[i]>>4;
//			Dis_DATE[i*2+1] = DATE_Buf[i]&0x0F;		
//		}
//		SBUF = Dis_DATE[4];
//		while(!TI);
//		TI = 0;
//		Display();	
//	}
//}