/*********************************************************************************************		
程序名: 	一位共阴数码管驱动程序  显示0~F
编写人: 　　 　
编写时间:	2014年7月30日　　
硬件支持:	STC12C4052AD
接口说明:　　 
修改日志:　　
　　NO.1-							
/*********************************************************************************************
说明:

/*********************************************************************************************/

#include <STC12C2052AD.H>//头文件

#define uchar unsigned char	//把 unsigned char 宏定义为 uchar

sbit g1 = P3^4;	//定义单片机引脚P3.4为g1
sbit g2 = P3^5;	//定义单片机引脚P3.5为g2


/**********************************************************************************************/
//定义延时函数
void delay_ms (unsigned int a)	//调用格式为"delay_ms(n)" n=0~4294967295 1000ms=1s"
{	unsigned int i;
	while( a-- != 0){
		for(i = 0; i < 600; i++); 
	}   				   
}

/**********************************************************************************************/
//共阴数码管 0~F 的字形码
uchar code tab[]={	
	0x3F,/*0*/
	0x06,/*1*/
	0x5B,/*2*/
	0x4F,/*3*/
	0x66,/*4*/
	0x6D,/*5*/
	0x7D,/*6*/
	0x07,/*7*/
	0x7F,/*8*/
	0x6F,/*9*/
	0x77,/*A*/
	0x7C,/*b*/
	0x39,/*C*/
	0x5E,/*d*/
	0x79,/*E*/
	0x71,/*F*/
};
/**********************************************************************************************/
//主函数
void main()
{
	P1M0 = 0x00;	//配置P1全部I/O口为强推挽输出
	P1M1 = 0xFF;

	while(1)
	{	int b;
		g1=0;		//设置P3.4为低电平
		g2=0;		//设置P3.5为低电平
		P1=tab[b];
		delay_ms(1000);	//1s延时
		b++;
	if(b==16)
		b=0;
	}
}
/**********************************************************************************************/

