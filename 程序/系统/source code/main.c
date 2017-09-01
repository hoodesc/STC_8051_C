#include <stc15.h>
#include "mytype.h"
#include "LCD12864.h"
#include "ds1302.h"
#include "delay.h"
#include "uart.h"
#include "nrf24l01_rx.h"
#include "init.h"
#include "a6.h"

uchar T[] = {0};
uchar MQ2[] = {0};
uchar HUO[] = {0};

void Bee();

sbit bee = P3^2;

//uchar HUOYAN[] = {0};
uchar discon[] = {0x20,0x20,0x20,0x20,0x20,0x20,'\0'};	//���Ͽ��� 16���Ʊ���
//uchar discon[] = {0xB6,0xCF,0xBF,0xAA,0x20,0x20,'\0'};	//���Ͽ��� 16���Ʊ���
bit T_Flag = 0;
bit MQ2_Flag = 0;
bit HUO_Flag = 0;
void Timer0_Init(void)		//10ms @11.0592MHz
{
	AUXR &= 0x7F;		//12Tģʽ
	TMOD &= 0xF0;		//ģʽ0 16λ�Զ���װ
	TL0 = 0x00;
	TH0 = 0xEE;
	ET0 = 1;
	EA  = 1;
	TR0 = 1;
}

void Timer0_ISR()	interrupt 1
{
	static uint time = 0;
	static uint mq2_rx_count = 0;
	static uint t_rx_count = 0;
	static uint huo_rx_count = 0;	
	static uchar mq2_sta = 0;
	static uchar huo_sta = 0;
	
	time++;
			SetRX_Mode();			
			NRF24L01_RxPacket(RX_DAT);
			if(RX_DAT[0] == 0xA0)	//	����0
			{
				T[0] = RX_DAT[1];
				T[1] = RX_DAT[2];
				T[2] = '.';
				T[3] = RX_DAT[3];
				T[4] = 0xA1;					
				T[5] = 0xE6;
				T[6] = '\0';
				T_Flag = 1;//�¶Ƚ������				
			}
			else if(RX_DAT[0] == 0xA1)	//	����1	
			{
				if(RX_DAT[1] == 0xAA)	//����
				{
					MQ2[0] = 0xD5;	//�������� 16���Ʊ���
					MQ2[1] = 0xFD;
					MQ2[2] = 0xB3;
					MQ2[3] = 0xA3;					
					MQ2[4] = '\0';
					MQ2[5] = 0xAA;
					MQ2_Flag = 1;	//MQ2������״̬�������
				}
				else if(RX_DAT[1] == 0xBB)	//��⵽�к�����
				{
					MQ2[0] = 0xBE;	//�����棡��16���Ʊ���
					MQ2[1] = 0xAF;
					MQ2[2] = 0xB8;
					MQ2[3] = 0xE6;
					MQ2[4] = '\0';
					MQ2[5] = 0xBB;
					MQ2_Flag = 1;	//MQ2������״̬�������	
				}			
			}
			else if(RX_DAT[0] == 0xA2)	//	����2	
			{
				if(RX_DAT[1] == 0xAA)	//����
				{
					HUO[0] = 0xD5;	//�������� 16���Ʊ���
					HUO[1] = 0xFD;
					HUO[2] = 0xB3;
					HUO[3] = 0xA3;					
					HUO[4] = '\0';
					HUO[5] = 0xAA;
					HUO_Flag = 1;	//���洫����״̬�������
				}
				else if(RX_DAT[1] == 0xBB)	//��⵽����
				{
					HUO[0] = 0xBE;	//�����棡��16���Ʊ���
					HUO[1] = 0xAF;
					HUO[2] = 0xB8;
					HUO[3] = 0xE6;
					HUO[4] = '\0';
					HUO[5] = 0xBB;
					HUO_Flag = 1;	//���洫����״̬�������
				}
			}			
			switch(huo_sta)
			{
				case 0:
				{
					if(HUO[5] == 0xBB)
						huo_sta = 1;
					else
						huo_sta = 0;
				}break;
				case 1:
				{
						Send_Message(huoyan_warning);
						huo_sta = 2;
				}break;
				case 2:
				{
						if(HUO[5] == 0xAA)
							huo_sta = 0;
						else
							huo_sta = 2;
				}break;
				default:break;
			}
			switch(mq2_sta)
			{
				case 0:
				{
					if(MQ2[5] == 0xBB)
						mq2_sta = 1;
					else
						mq2_sta = 0;
				}break;
				case 1:
				{
						Send_Message(mq2_warning);
						mq2_sta = 2;
				}break;
				case 2:
				{
						if(MQ2[5] == 0xAA)
							mq2_sta = 0;
						else
							mq2_sta = 2;
				}break;
				default:break;
			}			
			RX_DAT[0] = 0x00;RX_DAT[1] = 0x00;RX_DAT[2] = 0x00;RX_DAT[3] = 0x00;RX_DAT[4] = 0x00;
			if(T_Flag)
			{
				t_rx_count++;
				T_Flag = 0;				
				LCD_Display(1,1,T);				
			}
			if(MQ2_Flag)
			{
				mq2_rx_count++;
				MQ2_Flag = 0;
				LCD_Display(2,2,MQ2);
				if(MQ2[5] == 0xBB)
					Bee();
			}
			if(HUO_Flag)
			{
				huo_rx_count++;
				HUO_Flag = 0;
				LCD_Display(3,3,HUO);
				if(HUO[5] == 0xBB)
					Bee();				
			}			
			
		if(time > 200)	//	1000ms���һ��
		{
			time = 0;
			if(t_rx_count < 1)
			{
					if(!T_Flag)
					{
						LCD_Display(1,1,discon);
						DelayMs(1);					
					}
			}
			if(mq2_rx_count < 1)
			{
					if(!MQ2_Flag)
					{
						LCD_Display(2,2,discon);
						DelayMs(1);				
					}					
			}
			if(huo_rx_count < 1)
			{
					if(!HUO_Flag)
					{
						LCD_Display(3,3,discon);
						DelayMs(1);					
					}							
			}		
			mq2_rx_count = 0;			
			huo_rx_count = 0;
			t_rx_count = 0;
		}
		DS1302_Time_Display();//��ȡDS1302ʱ�䲢��ʾ��Һ��
}

void Bee()
{
		bee = 0;
		DelayMs(300);
		bee = 1;
		DelayMs(300);
}
void main() 
{
		INIT();//	������ʼ��
		USART1_Init();//���ڳ�ʼ��
		Wait_CREG();//�ȴ�GSMע��ɹ�
		Set_Text_Mode();//���÷���ΪTEXTģʽ
		Timer0_Init();				//	��ʱ��0��ʼ��
		DelayMs(1000);
		while(1)
		{


		}
}