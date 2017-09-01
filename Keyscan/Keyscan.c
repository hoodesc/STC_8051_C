#include<STC15.h>
//基于状态机 按键消抖程序

typedef unsigned char uchar;
typedef unsigned int uint;

typedef enum Key1_State{Init1_State,Affirm1_State,Single1_State,Repeat1_State};


sbit KEY1 = P3^3;
sbit KEY2 = P3^2;
sbit KEY3 = P3^1;
sbit Led1 = P3^5;
//
//#define key Key1

void KEY_Init(uchar k)
{
	static uchar Key1_StateValue = 0;	//按键1状态标志
	static uchar Time = 0;
	uchar Key1_Value = k;		//获取按键1
	
	switch (Key1_StateValue)
	{
		case Init1_State:	//初始化状态
		{
			if(!Key1_Value)
			{
				Key1_StateValue = Affirm1_State;	//下次进入确认状态
			}	
			else{Key1_StateValue = Init1_State;}	//否则进入初始化状态
		}
		break;
		case Affirm1_State:	//确认状态
		{
			if(!Key1_Value)
			{
				Time = 0;
				Key1_StateValue = Single1_State;	//下次进入单次触发状态
			}
			else{Key1_StateValue = Init1_State;}	//否则进入初始化状态
		}
		break;
		case Single1_State:	//单次触发状态
		{
			Time++;	//10ms 加一次
			if(Key1_Value)	//如果按键放开，LED取反，回到初始状态
			{
				Led1 = ~Led1;
				Key1_StateValue = Init1_State;					
			}
			else if(Time == 100)	//按下时间等于1S
			{
				Time = 0;
				Key1_StateValue = Repeat1_State;
			} 
		}
		break;
		case Repeat1_State:	//连发状态
		{
			Time++;
			if(10 == Time)
			{
			Time = 0;
			Led1 = ~Led1;
			}
			else if(Key1_Value)
			{
				Key1_StateValue = Init1_State;
			}					
		}
		break;
		default:Key1_StateValue = Init1_State;
		break;
		}
}


void Timer0_Init(void)
{
	TMOD = 0x01;
	TH0 = 0xD8;
	TL0 = 0xF0;
	TR0 = 1;
}

void main(void)
{
	Timer0_Init();
	P3M1 = 0x00;
	P3M0 = 0x00;
	while(1)
	{		
		if(TF0)	//溢出
		{
			TF0 = 0;
			TH0 = 0xD8;
			TL0 = 0xF0;
//			if(KEY1)
				KEY_Init(KEY1);
//			else if(KEY2)
//				KEY_Init(KEY2);
//			else if(KEY3)
//				KEY_Init(KEY3);
		}
		
	}
}