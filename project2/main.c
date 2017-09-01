#include <STC12C5A.h>
#include "MYTYPE.h"
#include "DS18B20.h"
#include "DS1302.h"
#include "LCD12864.h"
#include "MQ-2.h"
#include "GSM-GTM900b.h"
#include "Delay.h"
#include "Display.h"

sbit bee = P2^6;	// ������
uchar time = 0;
uchar System_Time = 0;
//void MQ2_scan();
void Timer0_Init(void)		//10����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
	ET0 = 1;
	EA  = 1;
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}

void Timer0_ISR()	interrupt 1
{
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
	time++;
//	if(time++ > 100)	// 1S
//	{
//		time = 0;
////		System_Time++;
//		if(!GSM_tsim_ok || !GSM_cops_ok || !GSM_csmp_ok || !GSM_csq_ok || !GSM_cnmi_ok)
//		{
//			//SIM�����		��Ӫ�̼��		SMS��ʽ			�źż��		������������
//			if(gsm_delay_time++ == 15)
//			{
//				gsm_delay_time = 0;
//				GSM_Init();		// ��ʼ��			
//			}							
//		}		
//	}
}

void Display()		//	��ʾ�̶�����
{
	uchar i;
	LCD_XY(4,0);	// �����С���һ��
	while(GSM_TAB[i] != '\0')
	{
		LCD_Write_Dat(GSM_TAB[i]);	// ��ǰ�źţ�
		i++;
	}
	i = 0;
	
	LCD_XY(2,2);	// �ڶ��е�����
	while(*(pGSM+i) != '\0')
	{
		LCD_Write_Dat(*(pGSM+i));
		i++;
	}
}

void main(void)
{
	static uchar value = 0;	

	Timer0_Init();		// ��ʱ��0��ʼ��
	LCD12864_Init();	// 12864Һ����ʼ��
	Display();		// ��ʾ�̶�����
	bee = 0;	// �ط�����
	DelayMs(2000);
		Uart_Init();		// ���ڳ�ʼ��.				
	while(1)
	{
		// ɨ���Ƿ��յ���Ϣ
		DS18B20_Display();			// ˢ���¶���ʾ
		Sensor_State_Display();		// ˢ�´�����״̬
		GSM_CSQ_COPS_Display();		// ˢ����Ӫ�̡��ź�ǿ��
		if(!GSM_tsim_ok || !GSM_cops_ok || !GSM_csmp_ok || !GSM_csq_ok || !GSM_cnmi_ok)
		{
			//SIM�����		��Ӫ�̼��		SMS��ʽ			�źż��		������������
			gsm_delay_time = 0;
			GSM_Init();		// ��ʼ��			
		}

		switch(value)
		{
			case 0:
			{
				if(MQ2)
				{
					MQ2_flag = 1;
					GSM_sms_ready(warning_gas);	//	����Ϣ
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