#include <STC12C5A.h>
#include "MYTYPE.h"
#include "DS18B20.h"
#include "DS1302.h"
#include "LCD12864.h"
#include "MQ-2.h"
#include "GSM-GTM900b.h"
#include "Delay.h"
#include "Display.h"

sbit bee = P2^6;	// 蜂鸣器
uchar time = 0;
uchar System_Time = 0;
//void MQ2_scan();
void Timer0_Init(void)		//10毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	ET0 = 1;
	EA  = 1;
	TR0 = 1;		//定时器0开始计时
}

void Timer0_ISR()	interrupt 1
{
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	time++;
//	if(time++ > 100)	// 1S
//	{
//		time = 0;
////		System_Time++;
//		if(!GSM_tsim_ok || !GSM_cops_ok || !GSM_csmp_ok || !GSM_csq_ok || !GSM_cnmi_ok)
//		{
//			//SIM卡检测		运营商检测		SMS格式			信号检测		短信提醒设置
//			if(gsm_delay_time++ == 15)
//			{
//				gsm_delay_time = 0;
//				GSM_Init();		// 初始化			
//			}							
//		}		
//	}
}

void Display()		//	显示固定内容
{
	uchar i;
	LCD_XY(4,0);	// 第四行、第一列
	while(GSM_TAB[i] != '\0')
	{
		LCD_Write_Dat(GSM_TAB[i]);	// 当前信号：
		i++;
	}
	i = 0;
	
	LCD_XY(2,2);	// 第二行第三列
	while(*(pGSM+i) != '\0')
	{
		LCD_Write_Dat(*(pGSM+i));
		i++;
	}
}

void main(void)
{
	static uchar value = 0;	

	Timer0_Init();		// 定时器0初始化
	LCD12864_Init();	// 12864液晶初始化
	Display();		// 显示固定内容
	bee = 0;	// 关蜂鸣器
	DelayMs(2000);
		Uart_Init();		// 串口初始化.				
	while(1)
	{
		// 扫描是否收到信息
		DS18B20_Display();			// 刷新温度显示
		Sensor_State_Display();		// 刷新传感器状态
		GSM_CSQ_COPS_Display();		// 刷新运营商、信号强度
		if(!GSM_tsim_ok || !GSM_cops_ok || !GSM_csmp_ok || !GSM_csq_ok || !GSM_cnmi_ok)
		{
			//SIM卡检测		运营商检测		SMS格式			信号检测		短信提醒设置
			gsm_delay_time = 0;
			GSM_Init();		// 初始化			
		}

		switch(value)
		{
			case 0:
			{
				if(MQ2)
				{
					MQ2_flag = 1;
					GSM_sms_ready(warning_gas);	//	发信息
					value = 1;
					time = 0;
				}				
			}
			break;
			case 1:
			{
				if(time > 200)
				{
					time = 0;
					if(!MQ2)
					{
						value = 0;
						MQ2_flag = 0;					
					}
				}		
			}
			break;
			default:
			break;		
		}
	}
}