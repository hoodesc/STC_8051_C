#include <reg52.h>
#include "mytype.h"
#include "intrins.h"
//sbit A1 = P1^0;
//sbit B1 = P1^1;
//sbit C1 = P1^2;
//sbit D1 = P1^3;
sbit A1 = P2^0;
sbit B1 = P2^1;
sbit C1 = P2^2;
sbit D1 = P2^3;


#define coil_A1 {A1 = 1;B1 = 0;C1 = 0;D1 = 0;}
#define coil_B1 {A1 = 0;B1 = 1;C1 = 0;D1 = 0;}
#define coil_C1 {A1 = 0;B1 = 0;C1 = 1;D1 = 0;} 
#define coil_D1 {A1 = 0;B1 = 0;C1 = 0;D1 = 1;}

void DelayMs(uint ValMS)
{
	uint uiVal,ujVal;
	for(uiVal = 0; uiVal < ValMS; uiVal++)
		for(ujVal = 0; ujVal < 120; ujVal++);
}

void main()
{
	while(1)
	{
		coil_A1;
		DelayMs(20);
		coil_B1;
		DelayMs(20);
		coil_C1;
		DelayMs(20);
		coil_D1;
		DelayMs(20);		
	}
}
