#include <reg52.h>
#include <stdio.h>
// NEC红外解码
// 11.0592
typedef unsigned char uchar;
typedef unsigned int uint;

uint IR_conunt = 0;			// 定时器计数
uchar IR_time[33];
uchar IR_data[4];
sbit LED = P1^7;

void LED()
bit conunt_ok = 0,decode_ok = 0;

void Uart_Init()	// 串口初始化
{
	TMOD &= 0x0F;
	TMOD |= 0x20;			// 定时器1 模式2 8位自动重装
	SCON |= 0x50;			// 方式1、可接收
	TH1   = 0xFD;			// 9600bps
	TL1   = 0xFD;
	ET1   = 0;				// 关闭定时器1中短
	TR1   = 1;				// 打开定时器1
}

void EX0_Init()	// 外部中断0初始化
{
	IT0 = 1;					// 下降沿触发
	EX0 = 1;    			// 开外部中断0
	EA  = 1;   			  // 开总中断
}
void EX0_ISR() interrupt 0
{
	static uchar num = 0;
	static bit start_flag = 0;
	if(!start_flag)
	{
		start_flag = 1;
		IR_conunt = 0;
	}
	else if(start_flag)
	{
		if(IR_conunt >= 32 && IR_conunt < 50)		// 判断是否为9.5ms+4.5ms引导码
			num = 0;
		IR_time[num] = IR_conunt;
		IR_conunt = 0;
		num++;
		if(33 == num)
		{
			num = 0;
			start_flag = 0;			// 第一次中断标志位
			conunt_ok = 1;			// 红外码接收完毕
		}
	}
}

void Timer0_Init()
{
	TMOD &= 0xF0;
	TMOD |= 0x02;
	TH0   = 0x00;
	TL0   = 0x00;
	ET0   = 1;
	TR0   = 1;
}
void Timer0_ISR()	interrupt 1
{
	IR_conunt++;			// 256 * 12/11.0592  记一次数
}

void IR_decode()
{
	uchar IR_byte;
	uchar i = 0,j = 0;
	uchar num = 1;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 8; j++)
		{
			if(IR_time[num] > 6)
			{
				IR_byte |= 0x80;
			}
			else
			{
				IR_byte = IR_byte;
			}
			if(j < 7)
				IR_byte >>= 1;
			num++;			
		}
		IR_data[i] = IR_byte;
		IR_byte = 0x00;
	}
	if(IR_data[2] == ~IR_data[3])
	{
		decode_ok = 1;
	}
}

//void LED()
//{
//	if(IR_data[2] == 0x45)
//	{
//		IR_data[2] = 0x00;
//		LED = ~LED;	
//	}
//}


void main()
{
	uchar i;
	Uart_Init();
	Timer0_Init();
	EX0_Init();
	while(1)
	{
		if(conunt_ok)			// 获取时间值是否完毕
		{
			conunt_ok = 0;
			IR_decode();		// 执行解码程序
			
			for(i = 0; i < 33; i++)			// 串口输出
			{
				TI = 1;
				printf("%d",(uint)IR_time[i]);
				while(!TI);
				TI = 0;
			}				
				TI = 1;
				printf("\n");
				while(!TI);
				TI = 0;	
			
				for(i = 0; i < 4; i++)			// 串口输出
				{
					TI = 1;
					printf("%x\n",(uint)IR_data[i]);
					while(!TI);
					TI = 0;
				}
		}
		
		if(decode_ok)			// 解码是否完毕
		{
			decode_ok = 0;
			if(IR_data[2] == 0x03 && IR_data[3] == 0xfc)
			{
				P1 = ~P1;
			}
		}
//		LED();		
	}
}