#include <stc15.h>
#include "mytype.h"
#include "uart.h"
#include "nrf24l01_tx.h"

/*  11.0592Mhz	Uart���ڳ�ʼ��		    */
void Uart_Init()	// ���ڳ�ʼ��
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR &= 0xFB;		//��ʱ��2ʱ��ΪFosc/12,��12T
	T2L = 0xE8;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
}
/**********************************/

/**********************************/
//	���ڵ���
void Uart_print(void)
{
	uchar temp = 0;
	temp = SPI_Read(CD);
	SBUF = temp;
	while(!TI);
	TI = 0;
}