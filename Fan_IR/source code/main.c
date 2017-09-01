#include <STC15.h>
#include "mytype.h"
#include "Delay.h"
#include "keyscan.h"
#include "main.h"
#include "intrins.h"
//	12.00Mhz
//	STC15L104W
uchar IR_Dat_Open[]  = {0,0,1,0,0,1,1,1,1,1,1};
uchar IR_Dat_Speed[] = {0,0,1,0,0,1,1,1,1,0,1};
uchar IR_Dat_Turn[]  = {0,0,1,0,0,0,1,1,1,0,0};

//	open分隔码  1.4ms  38K+7.1ms 
#define IRS1_L 106	
#define IRS1_H 550
//	speed、turn分隔码 0.55ms  38K+8ms 
#define IRS2_L 41	
#define IRS2_H 622

//	数据"0" 1.4ms 38K+0.3ms
#define IR0_L 106
#define IR0_H 25

//	数据"1" 0.55ms 38K+1.15ms
#define IR1_L 41
#define IR1_H 91

uint IR_Code_Time = 0;
bit IR_Code_Flag = 1;



void Timer2_Init(void)
{
	AUXR |= 0x04;		//?????1T??
	T2L = 0x64;		//??????
	T2H = 0xFF;		//??????
	AUXR |= 0x10;		//???2????
	IE2 = 0x04;
	EA  = 1;
}

void Timer2_ISR() interrupt 12
{
	IR_Code_Time++;
	if(IR_Code_Flag)
		IR_DAT = 1;
	else
		IR_DAT = ~IR_DAT;
}

void IR1_Code_Send(uchar *pdat)
{
	uchar i,j;
	uchar *IR_Temp;
	for(j = 0; j < 3; j++)
	{
		IR_Temp = pdat;
		for(i = 0; i < 11; i++)
		{
			if(IR_Temp[i])
			{	//	数据"1"		
				IR_Code_Time = 0;
				IR_Code_Flag = 0;
				while(IR_Code_Time < IR1_L);
				IR_Code_Time = 0;
				IR_Code_Flag = 1;
				while(IR_Code_Time < IR1_H);				
			}
			else
			{	//	数据"0"				
				IR_Code_Time = 0;
				IR_Code_Flag = 0;
				while(IR_Code_Time < IR0_L);
				IR_Code_Time = 0;
				IR_Code_Flag = 1;
				while(IR_Code_Time < IR0_H);
			}
		}
			//	分隔码
			IR_Code_Time = 0;
			IR_Code_Flag = 0;
			while(IR_Code_Time < IRS1_L);
			IR_Code_Time = 0;
			IR_Code_Flag = 1;
			while(IR_Code_Time < IRS1_H);
	}
}

void IR2_Code_Send(uchar *pdat)
{
	uchar i,j;
	uchar *IR_Temp;
	for(j = 0; j < 3; j++)
	{
		IR_Temp = pdat;
		for(i = 0; i < 11; i++)
		{
			if(IR_Temp[i])
			{	//	数据"1"		
				IR_Code_Time = 0;
				IR_Code_Flag = 0;
				while(IR_Code_Time < IR1_L);
				IR_Code_Time = 0;
				IR_Code_Flag = 1;
				while(IR_Code_Time < IR1_H);				
			}
			else
			{	//	数据"0"				
				IR_Code_Time = 0;
				IR_Code_Flag = 0;
				while(IR_Code_Time < IR0_L);
				IR_Code_Time = 0;
				IR_Code_Flag = 1;
				while(IR_Code_Time < IR0_H);
			}
		}
			//	分隔码
			IR_Code_Time = 0;
			IR_Code_Flag = 0;
			while(IR_Code_Time < IRS2_L);
			IR_Code_Time = 0;
			IR_Code_Flag = 1;
			while(IR_Code_Time < IRS2_H);
	}
}

void main()
{	
	Timer2_Init();
//	WKTCL = 50;		//掉电唤醒周期
//	WKTCH = 0x80;	//使能掉电唤醒定时器
	while(1)
	{
//			PCON = 0x02;
//			_nop_();
//			_nop_();
			Key1_Scan();
			Key2_Scan();
			Key3_Scan();
	}
}