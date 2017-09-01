#include <reg51.h>
/*
����11.0592Mhz
12T
��������Ч����
��ʱ��0 50ms����һ������ҡ�i������1
����20�����ʱ��i==20��50ms*20=1s
P1ȡ��
*/
void Timer0_Init(void)	//��ʱ��0��ʼ������
{
	TMOD = 0x01;	//���ö�ʱ��0����ʽ1
	TH0 = 0xD8;	    //װ��ֵ
	TL0 = 0xF0;		//��ʱ50ms
	
	EA = 1;			//�����ж�
	ET0 = 1;		//��ʱ��0����ж�����λ
	TR0 = 1;		//�򿪶�ʱ��0
}
void Timer0_ISR(void) interrupt 1	//��ʱ��0�жϷ������
{
	unsigned char i;
	i++;
	TH0 = 0xD8;		//����װ��ֵ
	TL0 = 0xF0;		//��ʱ10ms

	if(i == 100)		//10*100ms=1s
	{
		i = 0;
		P1 = ~P1;			
	}
}

void Timer1_Init(void)	//��ʱ��1��ʼ������
{
	TMOD = 0x10;	//���ö�ʱ��1����ʽ1
	TH1 = 0xD8;	    //װ��ֵ
	TL1 = 0xF0;		//��ʱ50ms
	
	EA = 1;			//�����ж�
	ET1 = 1;		//��ʱ��0����ж�����λ
	TR1 = 1;		//�򿪶�ʱ��0
}
void Timer1_ISR(void) interrupt 3	//��ʱ��0�жϷ������
{
	unsigned char i;
	i++;
	TH1 = 0xD8;		//����װ��ֵ
	TL1 = 0xF0;		//��ʱ10ms

	if(i == 100)		//10*100ms=1s
	{
		i = 0;
		P3 = ~P3;	
	}
}

void main(void)
{
	Timer0_Init();	//���ö�ʱ��0��ʼ������
	//Timer1_Init();
	while(1)
	{}	
}