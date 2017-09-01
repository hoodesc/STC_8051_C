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
void GSM_CSQ_COPS_Display()		// 营运商、信号强度显示
{
//		开机等15秒左右再执行GSM init
	
		if(GSM_CUCC)
			pGSM = CUCC;
		else if(!GSM_CUCC && !GSM_CMCC)
			pGSM = No_service;		// 无服务
		LCD_XY(2,2);	// 第一行第五列
		while(*(pGSM+i) != '\0')
		{
			LCD_Write_Dat(*(pGSM+i));
			i++;
		}
		i = 0;
			// 		获取GSM信号强度 显示
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
	LCD_XY(1,0);	// 第一行第一列			
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
					DS18B20_flag = 1;			// 温度过高
					GSM_sms_ready(warning_t);	// 发送信息
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
	LCD_XY(1,5);	// 第一行第五列
	while(c[i] != '\0')
	{
		LCD_Write_Dat(c[i]);
		i++;
	}
	i = 0;
}