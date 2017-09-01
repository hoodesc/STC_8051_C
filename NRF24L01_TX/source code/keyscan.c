#include <STC15.h>
#include "mytype.h"
#include "keyscan.h"
#include "delay.h"
#include "nrf24l01_tx.h"

//sbit Key1 = P3^7;
sbit Key1 = P3^5;
void Keyscan(void)
{
	static uchar Key1_StateValue = 0;	//按键1状态标志
	static uchar Time = 0;
	uchar Key1_Value = Key1;		//获取按键1
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
				SPI_Write_Buf(W_TX_PAYLOAD,TX_DAT,TX_PLOAD_WIDTH);	//	装载数据
				NRF24L01_TxPacket(TX_DAT);							//	发送
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