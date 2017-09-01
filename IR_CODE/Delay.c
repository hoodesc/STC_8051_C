#include<stc12c2052ad.h>
#include<intrins.h>
#include"MYTYPE.h"
#include"Delay.h"

//	11.0592延时函数，单位ms
void DelayMs(uint ValMS)
{
	uint uiVal,ujVal;
	for(uiVal = 0; uiVal < ValMS; uiVal++)
		for(ujVal = 0; ujVal < 120; ujVal++);
}
////	11.0592Mhz	约1us延时
void DelayUs(uint us)		
{	
	while(us--)
		_nop_();
}