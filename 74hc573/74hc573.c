#include <reg51.h>


sbit Wei = P3^7;
sbit GY = P3^5;


void delay_ms (unsigned int a)
{	unsigned int i;
	while( a-- != 0){
		for(i = 0; i < 600; i++); 
	}   				   
}

unsigned char code tab[]={	
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


void main()
{ 
	while(1)
	{	int b;
		GY = 0;
		Wei = 1;
		P1=tab[b];
		delay_ms(1000);	//1s延时

		b++;
	if(b==16)
		b=0;
	}
}

