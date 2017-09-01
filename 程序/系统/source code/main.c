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
uchar discon[] = {0x20,0x20,0x20,0x20,0x20,0x20,'\0'};	//“断开” 16进制编码
//uchar discon[] = {0xB6,0xCF,0xBF,0xAA,0x20,0x20,'\0'};	//“断开” 16进制编码
bit T_Flag = 0;
bit MQ2_Flag = 0;
bit HUO_Flag = 0;
void Timer0_Init(void)		//10ms @11.0592MHz
{
	AUXR &= 0x7F;		//12T模式
	TMOD &= 0xF0;		//模式0 16位自动重装
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
			if(RX_DAT[0] == 0xA0)	//	器件0
			{
				T[0] = RX_DAT[1];
				T[1] = RX_DAT[2];
				T[2] = '.';
				T[3] = RX_DAT[3];
				T[4] = 0xA1;					
				T[5] = 0xE6;
				T[6] = '\0';
				T_Flag = 1;//温度接收完成				
			}
			else if(RX_DAT[0] == 0xA1)	//	器件1	
			{
				if(RX_DAT[1] == 0xAA)	//正常
				{
					MQ2[0] = 0xD5;	//“正常” 16进制编码
					MQ2[1] = 0xFD;
					MQ2[2] = 0xB3;
					MQ2[3] = 0xA3;					
					MQ2[4] = '\0';
					MQ2[5] = 0xAA;
					MQ2_Flag = 1;	//MQ2传感器状态接收完成
				}
				else if(RX_DAT[1] == 0xBB)	//检测到有害气体
				{
					MQ2[0] = 0xBE;	//“警告！”16进制编码
					MQ2[1] = 0xAF;
					MQ2[2] = 0xB8;
					MQ2[3] = 0xE6;
					MQ2[4] = '\0';
					MQ2[5] = 0xBB;
					MQ2_Flag = 1;	//MQ2传感器状态接收完成	
				}			
			}
			else if(RX_DAT[0] == 0xA2)	//	器件2	
			{
				if(RX_DAT[1] == 0xAA)	//正常
				{
					HUO[0] = 0xD5;	//“正常” 16进制编码
					HUO[1] = 0xFD;
					HUO[2] = 0xB3;
					HUO[3] = 0xA3;					
					HUO[4] = '\0';
					HUO[5] = 0xAA;
					HUO_Flag = 1;	//火焰传感器状态接收完成
				}
				else if(RX_DAT[1] == 0xBB)	//检测到火焰
				{
					HUO[0] = 0xBE;	//“警告！”16进制编码
					HUO[1] = 0xAF;
					HUO[2] = 0xB8;
					HUO[3] = 0xE6;
					HUO[4] = '\0';
					HUO[5] = 0xBB;
					HUO_Flag = 1;	//火焰传感器状态接收完成
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
			
		if(time > 200)	//	1000ms检测一次
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
		DS1302_Time_Display();//读取DS1302时间并显示在液晶
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
		INIT();//	器件初始化
		USART1_Init();//串口初始化
		Wait_CREG();//等待GSM注册成功
		Set_Text_Mode();//设置发送为TEXT模式
		Timer0_Init();				//	定时器0初始化
		DelayMs(1000);
		while(1)
		{


		}
}