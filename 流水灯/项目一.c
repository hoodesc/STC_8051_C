#include<STC12C2052AD.H> //ͷ�ļ�
sbit LED1 = P3 ^ 0;
sbit LED2 = P3 ^ 1;
sbit LED3 = P3 ^ 2;
sbit LED4 = P3 ^ 3;
sbit LED5 = P3 ^ 4;
sbit LED6 = P3 ^ 5;
//���6��
sbit LED7 = P1 ^ 7;
sbit LED8 = P1 ^ 6;
sbit LED9 = P1 ^ 5;
sbit LED10 = P1 ^ 4;
sbit LED11 = P1 ^ 3;
sbit LED12 = P1 ^ 2;
sbit LED13 = P1 ^ 1;
sbit LED14 = P1 ^ 0;
//�ұ�8��
sbit KEY = P3 ^ 7; //����

/*********************************************************************************************
�����������뼶CPU��ʱ����
��  �ã�DELAY_MS (?);
��  ����1~65535����������Ϊ0��
����ֵ����
��  ����ռ��CPU��ʽ��ʱ�������ֵ��ͬ�ĺ���ʱ��
��  ע��Ӧ����1T��Ƭ��ʱi<600��Ӧ����12T��Ƭ��ʱi<125
/*********************************************************************************************/
void DELAY_MS (unsigned int a){
	unsigned int i;
	while( a-- != 0){
		for(i = 0; i < 600; i++);
	}
}
/*********************************************************************************************/
void main (void){   
	unsigned char b;
	for(b=0;;b++){
	unsigned char a;
		for(a=0;a<1;){
		LED1=a;
		a=1;
		DELAY_MS(100);
		LED1=a;
		DELAY_MS(100);
		}
		a=0;
		for(a=0;a<1;){
		LED2=a;
		DELAY_MS(100);
		a=1;
		LED2=a;
		DELAY_MS(100);
		}
		for(a=0;a<1;){
		LED3=a;
		DELAY_MS(100);
		a=1;
		LED3=a;
		DELAY_MS(100);
		}
		for(a=0;a<1;){
		LED4=a;
		DELAY_MS(100);
		a=1;
		LED4=a;
		DELAY_MS(100);
		}
		for(a=0;a<1;){
		LED1=a;
		LED2=a;
		LED3=a;
		LED4=a;
		DELAY_MS(100);
		a=1;
		LED1=a;
		LED2=a;
		LED3=a;
		LED4=a;
		DELAY_MS(100);
		}
	}
}