#include<STC12C5A.h>
#include"MYTYPE.h"
#include"LCD12864.h"
#include"delay.h"

#define COM 0xF8		  
#define DAT 0xFA

uchar code WenDu[] = "温度报警";
uchar code QiTi[] = "气体报警";
uchar code JingGao[] = "警告！！";
uchar code ZhengChang[] = "正常    ";
uchar code CMCC[] = "中国移动";
uchar code CUCC[] = "中国联通";
uchar code No_service[] = "无服务";


sbit LCD_CS = P2^2;		// 串行片选
sbit LCD_SID = P2^3;	// 串行数据
sbit LCD_CLK = P2^4;	// 串行同步时钟
sbit LCD_PSB = P2^7;	// 并/串行接口选择 H-并行；L-串行
// DB0~DB7
// 0000 0000
// P0^6~P0^0



//	写数据函数
void LCD_Write_Com(uchar com)
{
	uchar MSB,LSB;
	MSB = (com & 0xF0);
	LSB = (com <<= 4);
	LCD_Write_Byte(COM);		// 写入指令
	LCD_Write_Byte(MSB);		// 写入指令高8位
	LCD_Write_Byte(LSB);		// 写入指令低8位
}

//	写指令函数
void LCD_Write_Dat(uchar dat)
{
	uchar MSB,LSB;
	MSB = (dat & 0xF0);
	LSB = (dat <<= 4);
	LCD_Write_Byte(DAT);		// 写入数据
	LCD_Write_Byte(MSB);		// 写入数据高8位
	LCD_Write_Byte(LSB);		// 写入数据低8位
}

//	写入一字节
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
		uX = 0x80;		// 第一行
	else if(2 == x)
		uX = 0x90;		// 第二行
	else if(3 == x)
		uX = 0x88;		// 第三行
	else if(4 == x)		
		uX = 0x98;		// 第四行
	XY = uX+y;
	LCD_Write_Com(XY);
}

void LCD12864_Init()
{
	LCD_PSB = 0;
	LCD_CS = 1;		
	LCD_Write_Com(0x30);		// 功能设置：基本指令
	DelayMs(10);
	LCD_Write_Com(0x01);		// 清屏
	DelayMs(10);
	LCD_Write_Com(0x0C);		// 显示状态：开显示，游标关闭
	DelayMs(10);	
}