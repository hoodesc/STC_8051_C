#include<STC12C5A.h>
#include"MYTYPE.h"
#include"LCD12864.h"
#include"delay.h"

#define COM 0xF8		  
#define DAT 0xFA

uchar code WenDu[] = "�¶ȱ���";
uchar code QiTi[] = "���屨��";
uchar code JingGao[] = "���棡��";
uchar code ZhengChang[] = "����    ";
uchar code CMCC[] = "�й��ƶ�";
uchar code CUCC[] = "�й���ͨ";
uchar code No_service[] = "�޷���";


sbit LCD_CS = P2^2;		// ����Ƭѡ
sbit LCD_SID = P2^3;	// ��������
sbit LCD_CLK = P2^4;	// ����ͬ��ʱ��
sbit LCD_PSB = P2^7;	// ��/���нӿ�ѡ�� H-���У�L-����
// DB0~DB7
// 0000 0000
// P0^6~P0^0



//	д���ݺ���
void LCD_Write_Com(uchar com)
{
	uchar MSB,LSB;
	MSB = (com & 0xF0);
	LSB = (com <<= 4);
	LCD_Write_Byte(COM);		// д��ָ��
	LCD_Write_Byte(MSB);		// д��ָ���8λ
	LCD_Write_Byte(LSB);		// д��ָ���8λ
}

//	дָ���
void LCD_Write_Dat(uchar dat)
{
	uchar MSB,LSB;
	MSB = (dat & 0xF0);
	LSB = (dat <<= 4);
	LCD_Write_Byte(DAT);		// д������
	LCD_Write_Byte(MSB);		// д�����ݸ�8λ
	LCD_Write_Byte(LSB);		// д�����ݵ�8λ
}

//	д��һ�ֽ�
void LCD_Write_Byte(uchar dat)
{
	uchar i;
	for(i = 0; i < 8; i++)
	{
		LCD_CLK = 0;
		if(dat & 0x80)
			LCD_SID = 1;
		else
			LCD_SID = 0;
		LCD_CLK = 1;
		dat <<= 1;
	}
}

void LCD_XY(uchar x,y)
{
	uchar uX,XY;
	if(1 == x)
		uX = 0x80;		// ��һ��
	else if(2 == x)
		uX = 0x90;		// �ڶ���
	else if(3 == x)
		uX = 0x88;		// ������
	else if(4 == x)		
		uX = 0x98;		// ������
	XY = uX+y;
	LCD_Write_Com(XY);
}

void LCD12864_Init()
{
	LCD_PSB = 0;
	LCD_CS = 1;		
	LCD_Write_Com(0x30);		// �������ã�����ָ��
	DelayMs(10);
	LCD_Write_Com(0x01);		// ����
	DelayMs(10);
	LCD_Write_Com(0x0C);		// ��ʾ״̬������ʾ���α�ر�
	DelayMs(10);	
}