#include "stc15.h"
#include "keyscan.h"
#include "mytype.h"
#include "intrins.h"


typedef enum IR_Function{open = 0x13F << 5,speed = 0x13D << 5,turn = 0x11C << 5};

//uchar IR_Dat_Open[]  = {0,0,1,0,0,1,1,1,1,1,1};
//uchar IR_Dat_Speed[] = {0,0,1,0,0,1,1,1,1,0,1};
//uchar IR_Dat_Turn[]  = {0,0,1,0,0,0,1,1,1,0,0};

#define FOSC 11059200L

//-----------------------------------------------

#define F38_0KHz (65536-FOSC/2/38000)    //1T??
//#define F38_0KHz (65536-FOSC/2/12/38000) //12T??

//-----------------------------------------------

uint16 IR_Dat_Open = 0x13F << 5;
uint16 IR_Dat_Speed = 0x13D << 5;
uint16 IR_Dat_Turn = 0x11C << 5;

void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}

void IR_0()
{
	uint8 i = 0;
	INT_CLKO = 0x01;
	for(i = 130; i >0 ; i--)
		Delay10us();
	INT_CLKO = 0x00;
	for(i = 30; i >0 ; i--)
		Delay10us();
	INT_CLKO = 0x00;	
}

void IR_1()
{
	uint8 i = 0;
	INT_CLKO = 0x01;
	for(i = 48; i >0 ; i--)
		Delay10us();
	INT_CLKO = 0x00;
	for(i = 114; i >0 ; i--)
		Delay10us();
	INT_CLKO = 0x00;
}


void IR_Send(uint16 ir_dat)
{
	uint16 i,j;
	uint16 ir_temp;
	for(j = 3; j > 0; j--)
	{
		ir_temp = ir_dat;
		for(i = 11; i > 0; i--)
		{
			if(ir_temp & 0x8000)
				IR_1();
			else
				IR_0();
			ir_temp <<= 1;
		}
		switch(ir_dat)
		{
			case open:
			{
				INT_CLKO = 0x01;
				for(i = 118; i >0 ; i--)
					Delay10us();
				INT_CLKO = 0x00;
				for(i = 645; i >0 ; i--)
					Delay10us();
				INT_CLKO = 0x00;			
			}break;
			case speed: case turn:
			{
				INT_CLKO = 0x01;
				for(i = 44; i >0 ; i--)
					Delay10us();
				INT_CLKO = 0x00;
				for(i = 713; i >0 ; i--)
					Delay10us();
				INT_CLKO = 0x00;
			}break;
			default:break;
		}		
	}
}



void main()
{
    P3M0 = 0x00;
    P3M1 = 0x00;
	
		WKTCL = 99;		//掉电唤醒周期
		WKTCH = 0x80;	//使能掉电唤醒定时器
	
    AUXR |= 0x80;                   //???0?1T??
//  AUXR &= ~0x80;                  //???0?12T??

    TMOD = 0x00;                    //????????0(16??????)
    
    TMOD &= ~0x04;                  //C/T0=0, ???????????
//  TMOD |= 0x04;                   //C/T0=1, ?T0?????????????

    TL0 = F38_0KHz;                 //??????
    TH0 = F38_0KHz >> 8;
    TR0 = 1;

    while (1)
		{
			PCON = 0x02;			//进入掉电模式
			_nop_();_nop_();
			Key1_Scan();
			Key2_Scan();
			Key3_Scan();
		}
}
