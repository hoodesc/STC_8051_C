#include<STC15.h>
#include"MYTYPE.h"
#include"DS18B20.h"
#include"DS1302.h"
#include"LCD12864.h"
#include"MQ-2.h"
#include"GSM-GTM900b.h"
#include"Delay.h"
#include"Display.h"

uchar *pTAB = ZhengChang;
uchar *pGSM = No_service;
uchar i;
void GSM_CSQ_COPS_Display()		// Ӫ���̡��ź�ǿ����ʾ
{
//		������15��������ִ��GSM init
	
		if(GSM_CUCC)
			pGSM = CUCC;
		else if(!GSM_CUCC && !GSM_CMCC)
			pGSM = No_service;		// �޷���
		LCD_XY(2,2);	// ��һ�е�����
		while(*(pGSM+i) != '\0')
		{
			LCD_Write_Dat(*(pGSM+i));
			i++;
		}
		i = 0;
			// 		��ȡGSM�ź�ǿ�� ��ʾ
		LCD_XY(4,5);
		while(CSQ[i] != '\0')
		{
			LCD_Write_Dat(CSQ[i]);
			i++;
		}
		i = 0;	
}

void Sensor_State_Display()
{
	if(MQ2_flag && DS18B20_flag)
		pTAB = JingGao;
	else if(MQ2_flag)
		pTAB = QiTi;				
	else if(DS18B20_flag)
		pTAB = WenDu;				
	else
		pTAB = ZhengChang;
	LCD_XY(1,0);	// ��һ�е�һ��			
	while(*(pTAB+i) != '\0')
	{
		LCD_Write_Dat(*(pTAB+i));
		i++;
	}
	i = 0;
}

void DS18B20_Display()
{
	static uchar value = 0;
	DS18B20_Temperature_Read();
	if(c[0] != '8' && c[0] != '0' && c[0] != '1' && c[0] != '2')
	{
		switch(value)
		{
			case 0:
			{
				if(c[1] == '3')
				{
					DS18B20_flag = 1;			// �¶ȹ���
					GSM_sms_ready(warning_t);	// ������Ϣ
					value = 1;		
				}
				else
					DS18B20_flag = 0;
			}
			break;
			case 1:
			{
				if(c[1] != '3')
				{
					DS18B20_flag = 0;
					value = 0;				
				}						
			}
			break;
			default:
			break;		
		}
	}
	else
		DS18B20_flag = 0;
	LCD_XY(1,5);	// ��һ�е�����
	while(c[i] != '\0')
	{
		LCD_Write_Dat(c[i]);
		i++;
	}
	i = 0;
}