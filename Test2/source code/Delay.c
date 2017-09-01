#include<stc12c2052ad.h>
#include<intrins.h>
#include"MYTYPE.h"
#include"Delay.h"

//	11.0592延时函数，单位ms
void DelayMs(uchar time)
{
	uchar i,j;
	while(time--)
	{
		for(i = 0; i < 9; i++)
			for(j = 0; j < 177; j++);
	}	
}
////	11.0592Mhz	约2us延时
//void delay(uint us)		
//{	
//	uint i;
//	for(i = 0; i < us; i++)
//		_nop_();
//}