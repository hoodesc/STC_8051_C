#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
//1602ʱ��
//12M����LCD D1-D7��P1^0-7

static uchar S_ge = 0;	  //��_��λ
static uchar S_shi = 0;	  //��_ʮλ
static uchar M_ge = 0;	//��_��λ
static uchar M_shi = 0;	//��_ʮλ
static uchar H_ge = 0;	  //ʱ_��λ
static uchar H_shi = 0;     //ʱ_ʮλ
uchar code Num[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39}; 	//0-9  ʮ������ 
uchar code TAB2[]="Time: ";
/***************��������***************/
void Timer(void);
void Write_data(uchar com);
void Write_com(uchar com);
void Refresh_Display(void);
void DelayMs (uint a);
/**************************************/
/***************����ӿ�***************/
sbit RS	= P3^5;
sbit RW	= P3^4;
sbit EN = P3^3;
/**************************************/

/***************��ʱ����***************/
void DelayMs (uint a){
	uint i;
	while( a-- != 0){
		for(i = 0; i < 125; i++);
	}
}
/**************************************/

/***************д���ݡ�ָ��***************/
void Write_data(uchar com)	//д����
{
	RS = 1;
	RW = 0;
	EN  = 1;
	DelayMs(1);
	P1 = com;			 
	EN = 0;
}

void Write_com(uchar com)	//дָ��
{
	RS = 0;
	RW = 0;
	EN  = 1;
	DelayMs(1);
	P1 = com;	 
	EN = 0;
}
/**************************************/

/***************ˢ����ʾ***************/
void Refresh_Display(void)		
{

//��̬��ʾ����xx��  ��:��  ��xx��  ��:��  ��xx��
//            H          M          S
	Write_com(0x86);		//��һ����ʾ	
	Write_data(Num[H_shi]);		//д ʱ ʮλ
	Write_data(Num[H_ge]);		//д ʱ ��λ
	Write_data(0x3A);	//д ʮ������":"
	Write_data(Num[M_shi]);	 	//д �� ʮλ
	Write_data(Num[M_ge]);	 	//д �� ��λ
	Write_data(0x3A);	//д ʮ������":"
	Write_data(Num[S_shi]);	 	//д �� ʮλ			 
	Write_data(Num[S_ge]);	 	//д �� ��λ	
}
/**************************************/

/***************��ʾ���ݴ���***************/
void Timer(void)
{
	static uchar S = 0;		//��_����
	static uchar M = 0;		//��_����
	static uchar H = 0;		//��_����
			//ʱ_����
	S++;	//��һ��
	if(60 == S)		//60�뵽��
	{
		S = 0;
		M++;	//��һ��
	}
	if(60 == M)		//60���ӵ���
	{
		M = 0;
		H++;	//��һСʱ
	}
	if(24 == H)		//24Сʱ����
	{
		H = 0;
	}
	S_ge = S%10;	//����, �� ��λ
	S_shi = S/10;	//����, �� ʮλ
	M_ge = M%10;		//����, �� ��λ
	M_shi = M/10;		//����, �� ʮλ
	H_ge = H%10;	//����, ʱ ��λ
	H_shi = H/10;	//����, ʱ ʮλ
}
/**************************************/

/***************LCD_��ʼ��***************/
void LCD_Init()
{	
	Write_com(0x38);	//�������ã�8λ�������ߣ�5X8��������ʾ
	Write_com(0x0C);	//��ʾ�����ؿ��ƣ�����ʾ����겻��ʾ������˸
	Write_com(0x06);	//��������ģʽ��AC�Զ���1����ʾ����λ
	Write_com(0x01);	//����
}
/**************************************/

/***************��ʱ��0_��ʼ��***************/
void Timer0_Init(void)
{
	TMOD = 0x01;   	//װ��ֵ
	TH0 = 0xD8;		//10ms
	TL0 = 0xF0;
	ET0 = 1;
	EA = 1;	
	TR0 = 1;
}

//��ʱ��0_�жϷ���������ʱ��ˢ����ʾ
void Timer0_ISR(void) interrupt 1	
{
	static uchar i = 0;
	i++;	//10ms ��1	
	TH0 = 0xD8;		//��װ��ֵ
	TL0 = 0xF0;		//10ms
	if(100 == i)		//1Sʱ�䵽��
	{
		i = 0;		
		Timer();	//1s����һ�Σ������롢�֡�ʱ		
	}
	Refresh_Display();	//ˢ����ʾ
}
/**************************************/

void main(void)
{
	uchar i;
	LCD_Init();
	DelayMs(50);
	Write_com(0x80);
	for(i = 0;i<6;i++)
	{
		
		Write_data(TAB2[i]);
	}	
	Timer0_Init();	
	while(1)
	{	
	}	
}