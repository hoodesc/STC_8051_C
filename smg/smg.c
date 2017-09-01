#include <STC12C2052AD.H>

//段定义段选
sbit a = P1^0;
sbit b = P1^1;
sbit c = P1^2;
sbit d = P1^3;
sbit e = P1^4;
sbit f = P1^5;
sbit g = P1^6;
sbit h = P1^7;

//定义位选
sbit gnd1 = P3^4;
sbit gnd2 = P3^5;




//0-F字形码
const unsigned char seg7[16] = 
{	0x3F,/*0*/
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

//延时函数
void ms_delay(unsigned int t)
{
	unsigned int i;
	for (t; t > 0;t--)
		for (i = 110;i > 0; i--)
		;
}

//主函数
	main()
	{
	int num = 0;
	
	//配置端口开漏输出
	P1M1 = 0X00;
	P1M0 = 0X00;
	P3M0 = 0x00;
	P3M1 = 0xff;
	while(1)
	{
	P1 = seg7[num];
	ms_delay(1000);
	num ++;
	if (num > 15)
		num = 0;
	}
}
