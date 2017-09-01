#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int
uint Num;	//ȫ�ֱ���Num���洢����+��-���������Ķ�ʱ����ֵ
uint Init_data = 5;
typedef enum Key1_State{InitState,AffirmState,SingleState,RepeatState};
typedef enum Key2_State{InitState2,AffirmState2,SingleState2,RepeatState2};

sbit PWM = P1^7;
sbit KEY1 = P1^6;
sbit KEY2 = P1^5;


void Timer0_Init(void)	    //�������_��ʱ��0
{
	TMOD = 0x01;	
	TH0 = 0xD8;		//10ms
	TL0 = 0xF0;
	TR0 = 1;
}

void Timer1_Init(void)		//�ߵ͵�ƽ�л�_��ʱ��1
{
	TMOD = 0x10;	
	TH1 = 0xD8;	//10msɨ��һ��
	TL1 = 0xF0;
	EA = 1;
	ET1 = 1;
	TR1 = 1;
}

void Timer1_ISR(void)	interrupt 3
{
	static uchar value = 1;
	switch (value)
	{
		case 0:
			value = 1;					//״ֵ̬����Ϊ1���´ν�����ߵ�ƽ
			TH1 = (65536-46636+Num)/256;		//��ȡ����ɨ��ֵNum�������ֵ
			TL1 = (65536-46636+Num)%256;
			PWM = 0;
		break;
		case 1:
			value = 0; 					//״ֵ̬����Ϊ0���´ν�����͵�ƽ
			TH1 = (65536-Num)/256;;		//��ȡ����ɨ��ֵNum�������ֵ
			TL1 = (65536-Num)%256;;
			PWM = 1;
		break;	
	}
}

uchar Key1_scan()		//����1ɨ��
{
	static uchar Time_1 = 0;		//ʱ���־��1��=10ms
	uchar KeyStateValue = 0;
	uchar Key1_Value;	//����1״̬��־
	Key1_Value = KEY1;


	switch (KeyStateValue)
	{
		case InitState:							//��ʼ��״̬
			if(0 == Key1_Value)			//����������
			{
				KeyStateValue = AffirmState;	//�´�_����ȷ��״̬
			}
			else {KeyStateValue = InitState;}
		break;
		case AffirmState:						//ȷ��״̬
			if(0 == Key1_Value)
			{
				KeyStateValue = SingleState;	//�´�_���뵥�δ���״̬	
			}
			else {KeyStateValue = InitState;}
		break;
		case SingleState:						//���δ���״̬
			Time_1++;
			if(Time_1 > 100)		////�������ʱ�����1S
			{
				Time_1 = 0;
			 	KeyStateValue = InitState;		//�´�_��������״̬
				
			}
			else if(1 == Key1_Value)
			{
			 	Init_data++;
			}
		break;
		case RepeatState:						//����״̬
			Time_1++;
			if(1 == Key1_Value)					//��������ͷ�
			{
			 	KeyStateValue = InitState;		//�´�_�����ʼ��״̬
			}
			if(Time_1 > 10)		//�������ʱ�����100ms
			{
				Time_1 = 0;
			 	Init_data++;
			}

			if(26 == Init_data)
			{
				Init_data = 5;
			}
		break;
		default:KeyStateValue = InitState;
		break;
		}
	return Init_data;
}


uchar Key2_scan()		//����2ɨ��
{
	static uchar Time_2 = 0;		//ʱ���־��1��=10ms
	uchar KeyStateValue = 0;
	uchar Key2_Value;
	Key2_Value = KEY2;

	switch (KeyStateValue)
	{
		case InitState2:							//��ʼ��״̬
			if(Key2_Value == 0)			//����������
			{
				KeyStateValue = AffirmState2;	//�´�_����ȷ��״̬
			}
			else {KeyStateValue = InitState2;}
		break;
		case AffirmState2:						//ȷ��״̬
			if(Key2_Value == 0)
			{
				KeyStateValue = SingleState2;	//�´�_���뵥�δ���״̬	
			}
			else {KeyStateValue = InitState2;}
		break;
		case SingleState2:						//���δ���״̬
			Time_2++;
			if(Time_2 > 100)		////�������ʱ�����1S
			{
				Time_2 = 0;
				KeyStateValue = RepeatState2;	//�´�_��������״̬
			}
			else if(1 == Key2_Value)
			{
			 	Init_data--;
				if(0 == Init_data)
				{
					Init_data = 1;				
				}
			}
		break;
		case RepeatState2:						//����״̬
			Time_2++;
			if(1 == Key2_Value)					//��������ͷ�
			{
			 	KeyStateValue = InitState2;		//�´�_�����ʼ��״̬
			}
			if(Time_2>10)							//�������ʱ�����100ms
			{
				Time_2 = 0;
			 	Init_data--;
				if(0 == Init_data)
				{
					Init_data = 1;
				}
			}			
		break;
		default:KeyStateValue = InitState2;
		break;
		}
	return Init_data;
}
	
void main()
{
	Timer0_Init();
	Timer1_Init();
	while(1)
	{
		if(1 == TF0)	//��ʱ��0_�����־λ
		{
			TF0 = 0;	//����
			if(0 == KEY1)
			{
			 	Key1_scan();
				Num =Key1_scan()*100;		//��
			}
			else if(0 == KEY2)
			{
			    Key2_scan();
				Num = Key2_scan()*100;		//��
			}
		}
	}
 	
}


