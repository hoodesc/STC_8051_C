#include <reg52.h>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;

sbit SCLK = P0^0;
sbit IO = P0^1;
sbit CE = P0^2;

uchar DATE[] = {22,56,30,16,5,12};	// ʱ �� �� �� �� ��
uchar code DATE_Write_Add[]= {0x84,0x82,0x80,0x8C,0x88,0x86};		// д�Ĵ���
uchar code DATE_Read_Add[] = {0x85,0x83,0x81,0x8D,0x89,0x87};		// ���Ĵ���
uchar code WEI[] = {0xFE,0xFD,0xFB,0xF7,0xFF};		// �����λ��
uchar code DUAN[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};	// ���������0~F ������

uchar DATE_Buf[5] = {0x00};
uchar Dis_DATE[11] = {0x00};
//	11.0592Mhz	Լ2us��ʱ
void delay(uint time)		
{	
	uint i;
	for(i = 0; i < time; i++)
		_nop_();
}
void Write_DS1302_Byte(uchar dat)
{
	uchar i;
	for(i = 0; i < 8; i++)
	{
	 	SCLK = 0;
		IO = dat & 0x01;
		SCLK = 1;
		dat >>= 1;
	}
}

void Write_DS1302(uchar add,uchar dat)
{
	CE = 0;
	delay(1);
	SCLK = 0;
	delay(1);
	CE = 1;		// ʹ��оƬ
	delay(1);
	Write_DS1302_Byte(add);
	Write_DS1302_Byte(dat);
	CE = 0;		// ʧ��оƬ
	delay(1);
	IO = 1;		// �ͷ�����
	SCLK = 1;	// �ͷ�ʱ��
}

uchar Read_DS1302_Byte(uchar add)
{
	uchar temp = 0x00;
	uchar i;
	CE = 0;
	delay(1);
	SCLK = 0;
	delay(1);
	CE = 1;		// ʹ��оƬ
	delay(1);
	Write_DS1302_Byte(add);
	for(i = 0; i < 8; i++)
	{	
		temp >>= 1;	
		SCLK = 0;		
		if(IO)	 
			temp = temp|0x80;
		SCLK = 1;		 
	}	
	CE = 0;		// ʧ��оƬ
	delay(1);
	IO = 1;		// �ͷ�����
	SCLK = 1;	// �ͷ�ʱ��
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
	}
}
/*  11.0592Mhz	Uart���ڳ�ʼ��		    */
void Uart_Init()	// ���ڳ�ʼ��
{
	TMOD &= 0x0F;	// ������ʱ��0���ã���ն�ʱ��1����
	TMOD |= 0x20;	// ��ʱ��1 ģʽ2
	SCON |= 0x50;	// ���ڷ�ʽ1��8λUART�����ʿɱ䣬�ɽ���
	TH1   = 0xFD;	// TL1�����TH1��ֵװ��
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// �رն�ʱ��1�ж�
	TR1   = 1;		// ����ʱ��1
}
/**********************************/

//	�������ʾ
void Display(void)
{
	uchar i;
	for(i = 0; i < 5; i++)
	{
		P1 = 0xFF;					// ����ʾ
		P0 = DUAN[Dis_DATE[i+2]];		// �Ͷ���
		P1 = WEI[i];				// ����ʾ
	}
}

void main(void)
{
	uchar i;
	Uart_Init();				// ���ڳ�ʼ��				
	Write_DS1302(0x8E,0x00);	// ȥ��д����
	//Set_DS1302_TIME();			// д��ʱ������
	Write_DS1302(0x8E,0x80);	// ��д����
	while(1)
	{
		for(i = 0; i < 6; i++)
		{
			DATE_Buf[i] = Read_DS1302_Byte(DATE_Read_Add[i]);
			Dis_DATE[i*2] = DATE_Buf[i]>>4;
			Dis_DATE[i*2+1] = DATE_Buf[i]&0x0F;		
		}
//		SBUF = Dis_DATE[4];
//		while(!TI);
//		TI = 0;
		Display();	
	}
}