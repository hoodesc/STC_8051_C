#include<STC12C5A.h>
#include<intrins.h>
#include"MYTYPE.h"
#include"Delay.h"

//	11.0592��ʱ��������λms
void DelayMs(uint ms)
{
	uchar i,j;
	while(ms--)
	{
		for(i = 0; i < 9; i++)
			for(j = 0; j < 177; j++);
	}	
}
//	11.0592Mhz	Լ2us��ʱ
void delay(uint us)		
{	
	uint i;
	for(i = 0; i < us; i++)
		_nop_();
}