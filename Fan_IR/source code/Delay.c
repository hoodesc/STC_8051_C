#include<stc15.h>
#include<intrins.h>
#include"MYTYPE.h"
#include"Delay.h"

//	11.0592��ʱ��������λms
void DelayMs(uint ValMS)
{
	uint uiVal,ujVal;
	for(uiVal = 0; uiVal < ValMS; uiVal++)
		for(ujVal = 0; ujVal < 120; ujVal++);
}
////	11.0592Mhz	Լ1us��ʱ
void DelayUs(uint us)		
{	
	while(us--)
		_nop_();
}