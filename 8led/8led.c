#include <reg51.h>

typedef unsigned char uchar;

sbit KEY = P3^0;
sbit SPK = P2^0;
sbit LED = P2^4;

/**********************************/
/*		毫秒延时函数								*/
void DelayMs(unsigned int ms_number)
{
	unsigned int i;
	unsigned char j;
	for(i=0;i<ms_number;i++) 
	{  
		for(j=0;j<200;j++); 
		for(j=0;j<102;j++); 
	}  
} 

void main()
{	
	while(1)
	{
			LED = 1;
//		if(KEY)
//		{
//			SPK = 1;
//			P1 = 0x00;
//			DelayMs(200);
//			P1 = 0xFF;
//			DelayMs(200);
//			SPK = 0;
//			DelayMs(20);
//		}
//		else
//		{
//			P1 = 0x00;
//			SPK = 1;
		
//		}
	}
}