#include<STC15.h>
#include"MYTYPE.h"
#include"huo.h"
#include"Delay.h"

sbit DOUT = P3^2;	// int0 
bit flag = 0;

bit Start (void)
{
	if(DOUT) 
	flag = 0;
	else 
	flag = 1;
	return flag; 	
}