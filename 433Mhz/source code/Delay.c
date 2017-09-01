#include <stc15.h>
#include <intrins.h>
#include "MYTYPE.h"
#include "Delay.h"

//	11.0592延时函数
void DelayMs(uint16 ms)
{
	uint16 uiVal,ujVal;
	for(uiVal = 0; uiVal < ms; uiVal++)
		for(ujVal = 0; ujVal < 120; ujVal++);
}
////	11.0592Mhz	约2us延时
void DelayUs(uint16 us)		
{	
	while(us--)
		_nop_();
}