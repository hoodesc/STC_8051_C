#include <STC15.h>
#include "mytype.h"
#include "LCD12864.h"
#include "delay.h"

uchar temp0[] = {"T:"};
uchar temp1[] = {"MQ2:"};
uchar temp2[] = {"火焰:"};

uchar code PHOTO[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

sbit LCD_RS = P1^1; //液晶：指令/数据选择信号	液晶第4脚
sbit LCD_RW = P1^0;	//液晶：读写选择信号		液晶第5脚
sbit LCD_EN = P1^2;	//液晶：读写选择信号		液晶第6脚

void LCD_Write_com(uchar com)  //   写指令 
{  
	LCD_RS=0;  
	P0=com;  
	DelayUs(5);  
	LCD_EN=1;  
	DelayUs(5);  
	LCD_EN=0; 
}   
void LCD_Write_data(uchar dat) // 写数据 
{  
	LCD_RS=1;  
	P0=dat;  
	DelayUs(5);  
	LCD_EN=1;  
	DelayUs(5);  
	LCD_EN=0; 
}   
void LCD_Init() //初始化  
{
	LCD_RW = 0; 
	LCD_EN=0;   
	LCD_Write_com(0x30);  //功能设定，基本指令集 
	DelayMs(10);	
	LCD_Write_com(0x01);  //清屏  
	DelayMs(10);  
	LCD_Write_com(0x0c);  //开显示  
	DelayMs(10);
	LCD_Display(1,0,&temp0);
	DelayUs(100);
	LCD_Display(2,0,&temp1);
	DelayUs(100);	
	LCD_Display(3,0,&temp2);	
}

void LCD_Display(uchar x,uchar y,uchar *pdat)
{
	uchar i = 0;
	LCD_XY(x,y);
	DelayUs(100);
	while(pdat[i] != '\0')
	{
		LCD_Write_data(pdat[i]);	
		i++;
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
	LCD_Write_com(XY);
}

void Photo(uchar *pPic)
{
	uchar i,j;
	LCD_Write_com(0x34);		// 扩充指令，关绘图显示
	DelayUs(200);
	// 先写上半屏
	for(i = 0; i < 32; i++)
	{
		LCD_Write_com(0x80 + i);	// 写水平坐标
		DelayUs(10);
		LCD_Write_com(0x80);		// 写垂直坐标 	
		DelayUs(10);
		for(j = 0; j < 16; j++)
		{
			LCD_Write_data(*pPic++);	
			DelayUs(10);
		}
	}
	// 再写下半屏
	for(i = 0; i < 32; i++)
	{
		LCD_Write_com(0x80 + i);	// 写水平坐标
		DelayUs(10);
		LCD_Write_com(0x88);		// 写垂直坐标 	
		DelayUs(10);
		for(j = 0; j < 16; j++)
		{
			LCD_Write_data(*pPic++);	
			DelayUs(10);
		}
	}
	LCD_Write_com(0x36);		// 扩充指令，开绘图显示
}

//void main() 
//{
//	uchar num;     
//	LCD_Init();
//	P0M1 = 0x00;
//	P0M0 = 0x00;    
//	for(num=0;num<6;num++)     
//	{  
//		LCD_Write_data(table[num]);  
//		delay(200);  
//	}  
//	LCD_Write_com(0x81+0x10); // 换行
//	for(num=0;num<11;num++) 
//	{  
//		LCD_Write_data(table1[num]); 
//		delay(200); 
//	}  
//	while(1); //程序在此处停下  
//}