#include<reg52.h>  
#define uchar unsigned char 
#define uint unsigned int 
uchar code table[]="我我我";  
uchar code table1[]="my xxx xxx!"; 
sbit lcden=P3^4; //液晶屏使能(EN)端
sbit lcdrs=P3^5; //液晶屏RS端  
uchar num;   
void delay(uint z) //延时函数 
{  
	uint x,y;  
	for(x=z;x>0;x--) 
	for(y=110;y>0;y--); 
}   
void write_com(uchar com)  //   写指令 
{  
	lcdrs=0;  
	P1=com;  
	delay(5);  
	lcden=1;  
	delay(5);  
	lcden=0; 
}   
void write_data(uchar date) // 写数据 
{  
	lcdrs=1;  
	P1=date;  
	delay(5);  
	lcden=1;  
	delay(5);  
	lcden=0; 
}   
void init() //初始化  
{  
	lcden=0;   
	write_com(0x30);  //功能设定，基本指令集  
	write_com(0x0c);  //开显示  
	write_com(0x01);  //清屏  
	write_com(0x06);  //进入点设定  
	write_com(0x81);  //设置启示位置，不写也行，因为默认的就是从第一行开始的   
}   
void main() 
{     
	init();    
	for(num=0;num<6;num++)     
	{  
		write_data(table[num]);  
		delay(200);  
	}  
	write_com(0x81+0x10); // 换行
	for(num=0;num<11;num++) 
	{  
		write_data(table1[num]); 
		delay(200); 
	}  
	while(1); //程序在此处停下  
}