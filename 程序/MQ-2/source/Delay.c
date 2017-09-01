#include<STC15.h>
#include<intrins.h>
#include"MYTYPE.h"
#include"Delay.h"

//	11.0592延时函数，单位ms
void DelayMs(uint ms)
{
	uchar i,j;
	while(ms--)
	{
		for(i = 0; i < 9; i++)
			for(j = 0; j < 177; j++);
	}	
}
//	11.0592Mhz	约2us延时
void DelayUs(uint us)		
{	
	uint i;
	for(i = 0; i < us; i++)
		_nop_();
}