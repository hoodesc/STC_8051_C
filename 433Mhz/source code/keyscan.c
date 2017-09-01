#include <stc15.h>
#include "mytype.h"
#include "main.h"
#include "keyscan.h"

typedef enum Key_State{Init_State,Affirm_State,Single_State,Repeat_State};

sbit KEY1 = P3^2;
sbit KEY2 = P3^1;
sbit KEY3 = P3^0;


void Key1_Scan()
{
	static uint8 Key_StateValue = 0;
	uint8 Key_Value = KEY1;
	switch(Key_StateValue)
	{
		case Init_State:
		{
			if(Key_Value)
				Key_StateValue = Init_State;
			else
				Key_StateValue = Affirm_State;
		}
		break;
		case Affirm_State:
		{
			if(Key_Value)
				Key_StateValue = Init_State;
			else
			{
				Key_StateValue = Single_State;			
			}			
		}
		break;
		case Single_State:
		{
			if(Key_Value)
			{	
				Key_StateValue = Init_State;
				/*∞¥º¸÷¥––*/

			}
			while(!KEY1)	//À… ÷ºÏ≤‚
				break;
		}
		break;
		default:Key_StateValue = Init_State;
		break;	
	}
}


void Key2_Scan()
{
	static uint8 Key_StateValue = 0;
	static uint8 Time = 0;
	uint8 Key_Value = KEY2;
	switch(Key_StateValue)
	{
		case Init_State:
		{
			if(Key_Value)
				Key_StateValue = Init_State;
			else
				Key_StateValue = Affirm_State;
		}
		break;
		case Affirm_State:
		{
			if(Key_Value)
				Key_StateValue = Init_State;
			else
			{
				Time = 0;
				Key_StateValue = Single_State;			
			}			
		}
		break;
		case Single_State:
		{
			if(Key_Value)
			{	
				Key_StateValue = Init_State;
				/*∞¥º¸÷¥––*/
				
			}
			while(!KEY2)	//À… ÷ºÏ≤‚
				break;			
		}
		break;
		default:Key_StateValue = Init_State;
		break;	
	}
}
void Key3_Scan()
{
	static uint8 Key_StateValue = 0;
	static uint8 Time = 0;
	uint8 Key_Value = KEY3;
	switch(Key_StateValue)
	{
		case Init_State:
		{
			if(Key_Value)
				Key_StateValue = Init_State;
			else
				Key_StateValue = Affirm_State;
		}
		break;
		case Affirm_State:
		{
			if(Key_Value)
				Key_StateValue = Init_State;
			else
			{
				Key_StateValue = Single_State;			
			}			
		}
		break;
		case Single_State:
		{
			Time++;
			if(Key_Value)
			{	
				Key_StateValue = Init_State;
				/*∞¥º¸÷¥––*/
				
			}
			while(!KEY3)	//À… ÷ºÏ≤‚
				break;			
		}
		break;
		default:Key_StateValue = Init_State;
		break;	
	}
}

//void Timer0_Init()
//{
//	AUXR &= 0x7F;		//?????12T??
//	TMOD &= 0xF0;		//???????
//	TL0 = 0x00;		//??????
//	TH0 = 0xDC;		//??????
//	TF0 = 0;		//??TF0??
//	TR0 = 1;		//???0????
//}