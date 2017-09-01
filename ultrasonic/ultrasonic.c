#include <reg52.h>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;

uint TIME;
uchar code TAB[] = {'0','1','2','3','4','5','6','7','8','9'};
uchar t[] = {'0','0','0','C','M','\n'};

sbit Tring = P1^7;		// 控制端
sbit Echo = P3^2;		// 接收端

//	11.0592Mhz	约2us延时
void delay(uint time)		
{	
	uint i;
	for(i = 0; i < time; i++)
		_nop_();
}
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

void Timer0_ISR() interrupt 1
{
	while(!Echo)	// 每进来一次判断接收端是否低电平
		TIME++;		// 时间++ 		1.085069444444444

	TR0 = 0;		// Echo变高时，关闭定时器
	EX0 = 1;		// 打开外部中断0
}

void Count()
{
	float temp;
	uchar i;
	uchar ge,shi,bai;
	temp = TIME * 1.085069444444444 / 58;
	i = temp;
	bai = i/100;  	
	shi = i%100/10;	
	ge  = i%100%10;
	t[0] = TAB[bai];
	t[1] = TAB[shi];
	t[2] = TAB[ge];

	TIME = 0;
}

void Printf()
{
	uchar i;
	for(i = 0; i < 6; i++)
	{
		SBUF = t[i];
		while(!TI);
		TI = 0;
	}
}
void Timer0_Init()
{
	TMOD &= 0xF0;		// 设置定时器模式
	TMOD |= 0x02;		// 设置定时器模式
	TL0 = 0xFF;			// 设置定时初值
	TH0 = 0xFF;			// 设置定时重载值
	TF0 = 0;			// 清除TF0标志
	ET0 = 1;			// 定时器0 允许中断
	TR0 = 0;
}

void Int0_ISR()	interrupt 0		
{
	EX0 = 0;		// 关闭外部中断0
	TR0 = 1;		// 打开定时器0、TIME开始计数	
}

void Int0_Init()	// 外部中断0初始化
{
	IT0 = 0;	// 外部中断0 下降沿触发
	EX0	= 1;	// 外部中断0 中断允许
	EA  = 1;	// 开总中断
} 

/*  11.0592Mhz	Uart串口初始化		    */
void Uart_Init()	// 串口初始化
{
	TMOD &= 0x0F;	// 保留后定时器0设置，清空定时器1设置
	TMOD |= 0x20;	// 定时器1 模式2
	SCON |= 0x50;	// 串口方式1，8位UART波特率可变，可接收
	TH1   = 0xFD;	// TL1溢出后将TH1的值装入
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// 关闭定时器1中断
	TR1   = 1;		// 开定时器1
}
/**********************************/

void main()
{
	Uart_Init();
	Int0_Init();
	Timer0_Init();
	while(1)
	{
		Tring = 1;		// 触发信号开始
		delay(10);		// 延时20us
		Tring = 0;		// 触发信号结束
		DelayMs(100);
		Count();
		Printf();
	}
}