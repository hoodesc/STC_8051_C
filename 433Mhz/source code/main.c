#include "stc15.h"
#include "keyscan.h"
#include "mytype.h"
#include "intrins.h"

sbit DAT433 = P1^1;
uint8 DATA[66] = {0,0,0,1,1,1,0,1,0,0,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1
,0,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,0,1,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0};

	
void Delay380us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 5;
	j = 18;
	do
	{
		while (--j);
	} while (--i);
}



void Delay420us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 5;
	j = 129;
	do
	{
		while (--j);
	} while (--i);
}


void Delay780us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 9;
	j = 96;
	do
	{
		while (--j);
	} while (--i);
}


void Delay830us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 9;
	j = 234;
	do
	{
		while (--j);
	} while (--i);
}

void Delay3600us()		//@11.0592MHz
{
	
	
	unsigned char i, j;

	_nop_();
	i = 39;
	j = 183;
	do
	{
		while (--j);
	} while (--i);
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



void start()
{
	uint8 i;
	for(i = 12; i > 0; i--)
	{
		DAT433 = 1;
		Delay380us();
		DAT433 = 0;
		Delay420us();
	}
	DAT433 = 0;
	Delay3600us();
}

void Dat_0()
{
	DAT433 = 1;
	Delay380us();
	DAT433 = 0;
	Delay830us();
}

void Dat_1()
{
	DAT433 = 1;
	Delay780us();
	DAT433 = 0;
	Delay420us();
}

void send_433(uint8 *pdat)
{
	uint8 i;
	for(i = 66; i > 0; i--)
	{
		if(*pdat == 1)
			Dat_1();
		else
			Dat_0();
		pdat++;
	}
}


void main()
{
	while(1)
	{	
		start();	
		send_433(DATA);	
		Delay10ms();
		while(1);
	}

}