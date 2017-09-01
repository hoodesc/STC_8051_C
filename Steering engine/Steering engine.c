#include<STC12C2052AD.h>
typedef unsigned int uint;
typedef unsigned char uchar;
sbit PWM=P1^7;//���PWM�ź�

void Timer0_Init(void)		//��ʱ��0��ʼ��
{
	TMOD = 0x01;
	TH0 = 0xF8;	//װ��ֵ
	TL0 = 0x30;	//��ʱ2ms
	ET0 = 1;
	EA = 0;
	TR0 = 0;	
}

void Timer0_ISR(void) interrupt 1	//��ʱ��0 �жϷ�����
{
	static uchar i = 0;
	i++;
	TR0 = 0;
	if(1 == i%2)	//�����Σ��ߵ�ƽ
	{
		PWM = 1;
		TH0 = 0xB9;	//��װ18ms��ֵ
		TL0 = 0xB0;	
		TR0 = 1;	//������ʱ��
	
	}
	if(0 == i%2)	//ż���Σ��͵�ƽ
	{
		PWM = 0;	
		TH0 = 0xF8;	////��װ2ms��ֵ
		TL0 = 0x30;
		TR0 = 1;	//������ʱ��
		i = 2;
	}
}
void main(void)//������
{
	P1M0 = 0x00;
	P1M1 = 0xFF;
	Timer0_Init();
	while(1)
	{}
}