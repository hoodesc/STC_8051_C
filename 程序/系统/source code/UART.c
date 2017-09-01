#include <stc15.h>
#include "mytype.h"
#include "uart.h"
#include "a6.h"



void UART1_SendData(uchar dat)
{
	ES=0;					//�ش����ж�
	SBUF=dat;			
	while(TI!=1);	//�ȴ����ͳɹ�
	TI=0;					//��������жϱ�־
	ES=1;					//�������ж�
}

void UART1_SendString(char *s)
{
	while(*s)//����ַ���������
	{
		UART1_SendData(*s++);//���͵�ǰ�ַ�
	}
}


/********************* UART1�жϺ���************************/
void UART1_int (void) interrupt 4
{
	if (RI)
    {
      RI = 0;                           //���RIλ
			Uart1_Buf[First_Int] = SBUF;  	  //�����յ����ַ����浽������
			First_Int++;                			//����ָ������ƶ�
			if(First_Int > Buf1_Max)       		//���������,������ָ��ָ�򻺴���׵�ַ
			{
				First_Int = 0;
			}
    }
    if (TI)
    {
        TI = 0;                          //���TIλ
    }
}

void USART1_Init(void)
{
	SCON = 0x50;
	AUXR |= 0x01;
	AUXR &= 0xFB;	
	T2L = 0xFE;
	T2H = 0xFF;
	AUXR |= 0x10;	
	ES  = 1;
	EA = 1;
}