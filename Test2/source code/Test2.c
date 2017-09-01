#include <stc12c2052ad.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned int uint;

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

void main(void)
{
	CMOD = 0x02; // 设置 PCA 定时器
	CL = 0x00;//清零
	CH = 0x00;
	CCAP0L = 0x40; //设置75%占空比
	CCAP0H = 0x40; // 当CL由ff变为00溢出时，CCAP0H装载到CCAP0L中，实现无干扰的更新PWM。
	CCAPM0 = 0x42; //0100,0010 设置使用PWM模式
	CR = 1; //开启 PCA 定时器.
	while(1){};
}