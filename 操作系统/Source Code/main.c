#include <RTX51TNY.h>
#include <STC15.h>

sbit LED = P2^4;

job0 ()	_task_ 0
{
	os_create_task(1);
	while(1)
	{
		LED = 1;
	}
}

job1 () _task_ 1
{
	while(1)
	{
	
	}
}