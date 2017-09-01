#include <reg52.h>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;

sbit IN1 = P1^0;
sbit IN2 = P1^1;
sbit IN3 = P1^2;
sbit IN4 = P1^3;
sbit PWMA = P1^4;
sbit PWMB = P1^5;
// 超声波模块位定义
sbit Tring = P3^7;		// 控制端
sbit Echo = P3^2;		// 接收端
//sbit LeftSenser1 = P0^3;    //前左红外
//sbit RightSenser1 = P0^2;   //前右红外
//sbit LeftSenser2 = P0^1;    //后左红外
//sbit RightSenser2 = P0^0;    //后右红外
uchar Uartdata;
uint TIME;
bit modify = 0;

void Count();

uchar code TAB[] = {0,1,2,3,4,5,6,7,8,9};
uchar t[] = {0,0,0,'C','M','\n'};

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

void go()
{
	IN1 = 1;IN2 = 0;
	IN3 = 1;IN4 = 0;
	PWMA = 1;
	PWMB = 1;
}

void back()
{
	IN1 = 0;IN2 = 1;
	IN3 = 0;IN4 = 1;
	PWMA = 1;
	PWMB = 1;
}

void stop()
{
	IN1 = 0;IN2 = 0;
	IN3 = 0;IN4 = 0;	
	PWMA = 1;
	PWMB = 1;
}

void left()
{
	IN1 = 1;IN2 = 0;
	IN3 = 0;IN4 = 1;
	PWMA = 1;
	PWMB = 1;
}

void right()
{
	IN1 = 0;IN2 = 1;
	IN3 = 1;IN4 = 0;
	PWMA = 1;
	PWMB = 1;
}

//以下为4路红外避障
//void infrared()
//{
//	if ((!RightSenser1) && LeftSenser1) // 如果右边假，左边真
//	{ 
//			left();
//	}
//	if(RightSenser1 && (!LeftSenser1)) // 如果左边假，右边真
//	{
//			right();
//	}
//	if((RightSenser1) && (LeftSenser1))  // 如果左右真
//	{
//		  go();
//	}
//	if((!RightSenser1) && (!LeftSenser1))  //如果左右假
//	{
//		  stop();
//	}
//}	
void control()
{
	switch(Uartdata)
	{
		case 'c':
			if(!modify)
			{	
				go();
				Tring = 1;		// 触发信号开始
				delay(10);		// 延时20us
				Tring = 0;		// 触发信号结束
				Count();		// 距离
				DelayMs(100);
			}			
			else if(0 == t[0])		// 距离百位为0
			{
				if(t[1] < 4)		// 距离十位小于4
				{
					back();
					DelayMs(500);	
					stop();
					modify = 0;		// 消除距离警告 	
				}
			}							
			break;
		case 'd':
			back();
			break;
		case 'e':
			stop();
			break;
		case 'a':
			left();
			break;
		case 'b':
			right();
			break;
		default:
			break;
	}
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
	if(0 == t[0])		// 距离百位为0
	{
		if(t[1] < 4)		// 距离十位小于4
			modify = 1;		// 距离警告
		
	}
	TIME = 0;			// 定时计数清零
}

void Timer0_ISR() interrupt 1
{
	while(!Echo)	// 每进来一次判断接收端是否低电平
		TIME++;		// 时间++ 		1.085069444444444

	TR0 = 0;		// Echo变高时，关闭定时器
	EX0 = 1;		// 打开外部中断0
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

void Uart_ISR() interrupt 4
{
	if(RI)
	{
		RI = 0;
		Uartdata = SBUF;
	}
}

void Uart_Init()
{
	TMOD &= 0x0F;
	TMOD |= 0x20;
	SCON |= 0x50;
	TH1   = 0xfd;
	TH1   = 0xfd;
	ET1   = 0;
	ES 	  = 1;
	EA    = 1;
	TR1   = 1;
}

void main()
{	
	Uart_Init();
	Timer0_Init();
	Int0_Init();
	while(1)
	{
		control();


//		infrared();
	}
}