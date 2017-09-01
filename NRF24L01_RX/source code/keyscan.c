#include <reg51.h>
#include "mytype.h"
#include "keyscan.h"
#include "delay.h"

sbit Key1 = P3^7;

sbit A1 = P3^2;
sbit B1 = P3^3;
sbit C1 = P3^4;
sbit D1 = P3^5;

#define coil_A1 {A1 = 1;B1 = 0;C1 = 0;D1 = 0;}
#define coil_B1 {A1 = 0;B1 = 1;C1 = 0;D1 = 0;}
#define coil_C1 {A1 = 0;B1 = 0;C1 = 1;D1 = 0;} 
#define coil_D1 {A1 = 0;B1 = 0;C1 = 0;D1 = 1;}
#define coil_close {A1 = 0;B1 = 0;C1 = 0;D1 = 0;}

void motor()
{
	uchar i;
		for(i = 0; i < 240; i++)
		{
			coil_A1;
			DelayMs(20);
			coil_B1;
			DelayMs(20);
			coil_C1;
			DelayMs(20);
			coil_D1;
			DelayMs(20);		
		
		}
		DelayMs(8000);
		for(i = 0; i < 240; i++)
		{
			coil_D1;
			DelayMs(20);
			coil_C1;
			DelayMs(20);
			coil_B1;
			DelayMs(20);
			coil_A1;
			DelayMs(20);				
		}
		coil_close;			
	}

void Keyscan(void)
{
	static uchar Key1_StateValue = 0;	//����1״̬��־
	static uchar Time = 0;
	uchar Key1_Value = Key1;		//��ȡ����1
	switch (Key1_StateValue)
	{
		case Init1_State:	//��ʼ��״̬
		{
			if(!Key1_Value)
			{
				Key1_StateValue = Affirm1_State;	//�´ν���ȷ��״̬
			}	
			else{Key1_StateValue = Init1_State;}	//��������ʼ��״̬
		}
		break;
		case Affirm1_State:	//ȷ��״̬
		{
			if(!Key1_Value)
			{
				Time = 0;
				Key1_StateValue = Single1_State;	//�´ν��뵥�δ���״̬
			}
			else{Key1_StateValue = Init1_State;}	//��������ʼ��״̬
		}
		break;
		case Single1_State:	//���δ���״̬
		{
			Time++;	//10ms ��һ��
			if(Key1_Value)	//��������ſ���LEDȡ�����ص���ʼ״̬
			{
				motor();
				Key1_StateValue = Init1_State;					
			}
			else if(Time == 100)	//����ʱ�����1S
			{
				Time = 0;
				Key1_StateValue = Repeat1_State;
			} 
		}
		break;
		case Repeat1_State:	//����״̬
		{
			Time++;
			if(10 == Time)
			{
				Time = 0;
//				Led1 = ~Led1;
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