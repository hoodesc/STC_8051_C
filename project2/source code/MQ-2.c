#include<STC12C5A.h>
#include"MYTYPE.h"
#include"MQ-2.h"
#include"Delay.h"


bit MQ2_flag = 0;

//void Int0_ISR()	interrupt 0		
//{
//	EX0 = 0;		// �ر��ⲿ�ж�0
//	MQ2_flag = 1;	// �����к������ʶ
//					// ������ѭ����⡰MQ2��ֱ���ȶ��͵�ƽ	
//}
//
//void Int0_Init()	// �ⲿ�ж�0��ʼ��
//{
//	IT0 = 1;	// �ⲿ�ж�0 �½��ش���
//	EX0	= 1;	// �ⲿ�ж�0 �ж�����
//	EA  = 1;	// �����ж�
//}