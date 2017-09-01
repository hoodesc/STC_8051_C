#include<reg52.h>  
#define uchar unsigned char 
#define uint unsigned int 
uchar code table[]="������";  
uchar code table1[]="my xxx xxx!"; 
sbit lcden=P3^4; //Һ����ʹ��(EN)��
sbit lcdrs=P3^5; //Һ����RS��  
uchar num;   
void delay(uint z) //��ʱ���� 
{  
	uint x,y;  
	for(x=z;x>0;x--) 
	for(y=110;y>0;y--); 
}   
void write_com(uchar com)  //   дָ�� 
{  
	lcdrs=0;  
	P1=com;  
	delay(5);  
	lcden=1;  
	delay(5);  
	lcden=0; 
}   
void write_data(uchar date) // д���� 
{  
	lcdrs=1;  
	P1=date;  
	delay(5);  
	lcden=1;  
	delay(5);  
	lcden=0; 
}   
void init() //��ʼ��  
{  
	lcden=0;   
	write_com(0x30);  //�����趨������ָ�  
	write_com(0x0c);  //����ʾ  
	write_com(0x01);  //����  
	write_com(0x06);  //������趨  
	write_com(0x81);  //������ʾλ�ã���дҲ�У���ΪĬ�ϵľ��Ǵӵ�һ�п�ʼ��   
}   
void main() 
{     
	init();    
	for(num=0;num<6;num++)     
	{  
		write_data(table[num]);  
		delay(200);  
	}  
	write_com(0x81+0x10); // ����
	for(num=0;num<11;num++) 
	{  
		write_data(table1[num]); 
		delay(200); 
	}  
	while(1); //�����ڴ˴�ͣ��  
}