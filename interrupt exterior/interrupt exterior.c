#include<stc12c2052ad.h>
//�ⲿ�ж�0
//P3.2�������£������ⲿ�жϣ��ⲿ�ж�0�������
//����P1.7 LED��

typedef unsigned char uchar;
typedef unsigned int uint;

sbit Init0 = P3^2;
sbit LED1 = P1^7;


//�ⲿ�ж�0�������
void Init0_ISR(void) interrupt 0
{
	EX0 = 0;		//��ֹ�ⲿ�ж�0�ж�
	LED1 = ~LED1;
	EX0 =  1;
}

void main(void)
{
	IT0 = 0;	//�ⲿ�ж�0���͵�ƽ����
	EX0 = 1;	//�����ⲿ�ж�0�ж�
	EA = 1;		//�����ж�
	while(1);
}