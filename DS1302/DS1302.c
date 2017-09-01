#include <reg52.h>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;

sbit SCLK = P0^0;
sbit IO = P0^1;
sbit CE = P0^2;

uchar DATE[] = {22,56,30,16,5,12};	// 时 分 秒 年 月 日
uchar code DATE_Write_Add[]= {0x84,0x82,0x80,0x8C,0x88,0x86};		// 写寄存器
uchar code DATE_Read_Add[] = {0x85,0x83,0x81,0x8D,0x89,0x87};		// 读寄存器
uchar code WEI[] = {0xFE,0xFD,0xFB,0xF7,0xFF};		// 数码管位码
uchar code DUAN[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};	// 共阳数码管0~F 不带点

uchar DATE_Buf[5] = {0x00};
uchar Dis_DATE[11] = {0x00};
//	11.0592Mhz	约2us延时
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
	CE = 1;		// 使能芯片
	delay(1);
	Write_DS1302_Byte(add);
	Write_DS1302_Byte(dat);
	CE = 0;		// 失能芯片
	delay(1);
	IO = 1;		// 释放数据
	SCLK = 1;	// 释放时钟
}

uchar Read_DS1302_Byte(uchar add)
{
	uchar temp = 0x00;
	uchar i;
	CE = 0;
	delay(1);
	SCLK = 0;
	delay(1);
	CE = 1;		// 使能芯片
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
	CE = 0;		// 失能芯片
	delay(1);
	IO = 1;		// 释放数据
	SCLK = 1;	// 释放时钟
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
/*  11.0592Mhz	Uart串口初始化		    */
void Uart_Init()	// 串口初始化
{
	TMOD &= 0x0F;	// 保留后定时器0设置，清空定时器1设置
	TMOD |= 0x20;	// 定时器1 模式2
	SCON |= 0x50;	// 串口方式1，8位UART波特率可变，可接收
	TH1   = 0xFD;	// TL1溢出后将TH1的值装入
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// 关闭定时器1中断
	TR1   = 1;		// 开定时器1
}
/**********************************/

//	数码管显示
void Display(void)
{
	uchar i;
	for(i = 0; i < 5; i++)
	{
		P1 = 0xFF;					// 关显示
		P0 = DUAN[Dis_DATE[i+2]];		// 送段码
		P1 = WEI[i];				// 开显示
	}
}

void main(void)
{
	uchar i;
	Uart_Init();				// 串口初始化				
	Write_DS1302(0x8E,0x00);	// 去除写保护
	//Set_DS1302_TIME();			// 写入时间日期
	Write_DS1302(0x8E,0x80);	// 加写保护
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