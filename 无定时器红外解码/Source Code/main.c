#include <stc15.h>

typedef unsigned char uint8;
typedef unsigned int uint16;

sbit IR = P3^2;
uint8 IR_DATA[4];

void Uart_Init(void)		//9600bps@24.000MHz
{
	SCON = 0x50;		//8???,?????
	AUXR |= 0x01;		//??1?????2???????
	AUXR |= 0x04;		//???2???Fosc,?1T
	T2L = 0x8F;		//??????
	T2H = 0xFD;		//??????
	AUXR |= 0x10;		//?????2
}

void Uart_Send(uint8 dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
}
void Delay100us()		//@24.000MHz
{
	unsigned char i, j;

	i = 3;
	j = 82;
	do
	{
		while (--j);
	} while (--i);
}

void Int0_Init(void)
{
	IT0 = 0;	//�ⲿ�ж� �½��ش���
	EX0 = 1;	//ʹ���ⲿ�ж�
	EA  = 1;	//ʹ�����ж�	
}

void Int0_ISR(void)	interrupt 0
{
	static uint8 time = 0;
	uint8 IR_Temp = 0,i = 0;
	EX0 = 0;
	while(!IR)
	{
		Delay100us();
		time++;
	}
	if(time < 80)		//������͵�ƽ < 8ms
	{
		EX0 = 1;	
		return;				//�����ж�
	}
	time = 0;
	while(IR)
	{
		Delay100us();
		time++;
	}
	if(time < 30)		//������ߵ�ƽ < 3ms
	{
		EX0 = 1;
		return;
	}

//�û���
	while(i++<32)
	{
		time = 0;
		IR_Temp >>= 1;
		while(!IR);		//�����͵�ƽ
		while(IR)
		{
			Delay100us();
			time++;
		}
		if(time > 10)
			IR_Temp |= 0x80;
		if(i==8)
			IR_DATA[0] = IR_Temp;		//�û���
		else if(i == 16)
			IR_DATA[1] = IR_Temp;		//�û�����
		else if(i == 24)			
			IR_DATA[2] = IR_Temp;		//������
		else if(i == 32)		
			IR_DATA[3] = IR_Temp;		//��������
	}
//У������
	if(IR_DATA[2] != ~IR_DATA[3])
	{
		EX0 = 1;
		return;
	}

		SBUF = IR_DATA[3];
		while(!TI);
		TI = 0;
		EX0 = 1;	
}



void main(void)
{
	Uart_Init();
	Int0_Init();
	while(1)
	{

	}
}