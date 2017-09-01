#include <reg52.h>
#include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;

uint TIME;
uchar code TAB[] = {'0','1','2','3','4','5','6','7','8','9'};
uchar t[] = {'0','0','0','C','M','\n'};

sbit Tring = P1^7;		// ���ƶ�
sbit Echo = P3^2;		// ���ն�

//	11.0592Mhz	Լ2us��ʱ
void delay(uint time)		
{	
	uint i;
	for(i = 0; i < time; i++)
		_nop_();
}
//	11.0592��ʱ��������λms
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
	while(!Echo)	// ÿ����һ���жϽ��ն��Ƿ�͵�ƽ
		TIME++;		// ʱ��++ 		1.085069444444444

	TR0 = 0;		// Echo���ʱ���رն�ʱ��
	EX0 = 1;		// ���ⲿ�ж�0
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
	TMOD &= 0xF0;		// ���ö�ʱ��ģʽ
	TMOD |= 0x02;		// ���ö�ʱ��ģʽ
	TL0 = 0xFF;			// ���ö�ʱ��ֵ
	TH0 = 0xFF;			// ���ö�ʱ����ֵ
	TF0 = 0;			// ���TF0��־
	ET0 = 1;			// ��ʱ��0 �����ж�
	TR0 = 0;
}

void Int0_ISR()	interrupt 0		
{
	EX0 = 0;		// �ر��ⲿ�ж�0
	TR0 = 1;		// �򿪶�ʱ��0��TIME��ʼ����	
}

void Int0_Init()	// �ⲿ�ж�0��ʼ��
{
	IT0 = 0;	// �ⲿ�ж�0 �½��ش���
	EX0	= 1;	// �ⲿ�ж�0 �ж�����
	EA  = 1;	// �����ж�
} 

/*  11.0592Mhz	Uart���ڳ�ʼ��		    */
void Uart_Init()	// ���ڳ�ʼ��
{
	TMOD &= 0x0F;	// ������ʱ��0���ã���ն�ʱ��1����
	TMOD |= 0x20;	// ��ʱ��1 ģʽ2
	SCON |= 0x50;	// ���ڷ�ʽ1��8λUART�����ʿɱ䣬�ɽ���
	TH1   = 0xFD;	// TL1�����TH1��ֵװ��
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// �رն�ʱ��1�ж�
	TR1   = 1;		// ����ʱ��1
}
/**********************************/

void main()
{
	Uart_Init();
	Int0_Init();
	Timer0_Init();
	while(1)
	{
		Tring = 1;		// �����źſ�ʼ
		delay(10);		// ��ʱ20us
		Tring = 0;		// �����źŽ���
		DelayMs(100);
		Count();
		Printf();
	}
}