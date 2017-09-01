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
// ������ģ��λ����
sbit Tring = P3^7;		// ���ƶ�
sbit Echo = P3^2;		// ���ն�
//sbit LeftSenser1 = P0^3;    //ǰ�����
//sbit RightSenser1 = P0^2;   //ǰ�Һ���
//sbit LeftSenser2 = P0^1;    //�������
//sbit RightSenser2 = P0^0;    //���Һ���
uchar Uartdata;
uint TIME;
bit modify = 0;

void Count();

uchar code TAB[] = {0,1,2,3,4,5,6,7,8,9};
uchar t[] = {0,0,0,'C','M','\n'};

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

//����Ϊ4·�������
//void infrared()
//{
//	if ((!RightSenser1) && LeftSenser1) // ����ұ߼٣������
//	{ 
//			left();
//	}
//	if(RightSenser1 && (!LeftSenser1)) // �����߼٣��ұ���
//	{
//			right();
//	}
//	if((RightSenser1) && (LeftSenser1))  // ���������
//	{
//		  go();
//	}
//	if((!RightSenser1) && (!LeftSenser1))  //������Ҽ�
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
				Tring = 1;		// �����źſ�ʼ
				delay(10);		// ��ʱ20us
				Tring = 0;		// �����źŽ���
				Count();		// ����
				DelayMs(100);
			}			
			else if(0 == t[0])		// �����λΪ0
			{
				if(t[1] < 4)		// ����ʮλС��4
				{
					back();
					DelayMs(500);	
					stop();
					modify = 0;		// �������뾯�� 	
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
	if(0 == t[0])		// �����λΪ0
	{
		if(t[1] < 4)		// ����ʮλС��4
			modify = 1;		// ���뾯��
		
	}
	TIME = 0;			// ��ʱ��������
}

void Timer0_ISR() interrupt 1
{
	while(!Echo)	// ÿ����һ���жϽ��ն��Ƿ�͵�ƽ
		TIME++;		// ʱ��++ 		1.085069444444444

	TR0 = 0;		// Echo���ʱ���رն�ʱ��
	EX0 = 1;		// ���ⲿ�ж�0
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