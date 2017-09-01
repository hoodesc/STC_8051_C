#include <STC15.h>
#include "mytype.h"
#include "keyscan.h"
#include "delay.h"
#include "nrf24l01_tx.h"

//sbit Key1 = P3^7;
sbit Key1 = P3^5;
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
				SPI_Write_Buf(W_TX_PAYLOAD,TX_DAT,TX_PLOAD_WIDTH);	//	װ������
				NRF24L01_TxPacket(TX_DAT);							//	����
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