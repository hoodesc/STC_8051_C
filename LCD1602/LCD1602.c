#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
//1602时钟
//12M晶振、LCD D1-D7接P1^0-7

static uchar S_ge = 0;	  //秒_个位
static uchar S_shi = 0;	  //秒_十位
static uchar M_ge = 0;	//分_个位
static uchar M_shi = 0;	//分_十位
static uchar H_ge = 0;	  //时_个位
static uchar H_shi = 0;     //时_十位
uchar code Num[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39}; 	//0-9  十六进制 
uchar code TAB2[]="Time: ";
/***************函数声明***************/
void Timer(void);
void Write_data(uchar com);
void Write_com(uchar com);
void Refresh_Display(void);
void DelayMs (uint a);
/**************************************/
/***************定义接口***************/
sbit RS	= P3^5;
sbit RW	= P3^4;
sbit EN = P3^3;
/**************************************/

/***************延时函数***************/
void DelayMs (uint a){
	uint i;
	while( a-- != 0){
		for(i = 0; i < 125; i++);
	}
}
/**************************************/

/***************写数据、指令***************/
void Write_data(uchar com)	//写数据
{
	RS = 1;
	RW = 0;
	EN  = 1;
	DelayMs(1);
	P1 = com;			 
	EN = 0;
}

void Write_com(uchar com)	//写指令
{
	RS = 0;
	RW = 0;
	EN  = 1;
	DelayMs(1);
	P1 = com;	 
	EN = 0;
}
/**************************************/

/***************刷新显示***************/
void Refresh_Display(void)		
{

//动态显示，“xx”  “:”  “xx”  “:”  “xx”
//            H          M          S
	Write_com(0x86);		//第一行显示	
	Write_data(Num[H_shi]);		//写 时 十位
	Write_data(Num[H_ge]);		//写 时 个位
	Write_data(0x3A);	//写 十六进制":"
	Write_data(Num[M_shi]);	 	//写 分 十位
	Write_data(Num[M_ge]);	 	//写 分 个位
	Write_data(0x3A);	//写 十六进制":"
	Write_data(Num[S_shi]);	 	//写 秒 十位			 
	Write_data(Num[S_ge]);	 	//写 秒 个位	
}
/**************************************/

/***************显示内容处理***************/
void Timer(void)
{
	static uchar S = 0;		//秒_变量
	static uchar M = 0;		//分_变量
	static uchar H = 0;		//分_变量
			//时_变量
	S++;	//加一秒
	if(60 == S)		//60秒到了
	{
		S = 0;
		M++;	//加一分
	}
	if(60 == M)		//60分钟到了
	{
		M = 0;
		H++;	//加一小时
	}
	if(24 == H)		//24小时到了
	{
		H = 0;
	}
	S_ge = S%10;	//分离, 秒 个位
	S_shi = S/10;	//分离, 秒 十位
	M_ge = M%10;		//分离, 分 个位
	M_shi = M/10;		//分离, 分 十位
	H_ge = H%10;	//分离, 时 个位
	H_shi = H/10;	//分离, 时 十位
}
/**************************************/

/***************LCD_初始化***************/
void LCD_Init()
{	
	Write_com(0x38);	//功能设置：8位数据总线，5X8，两行显示
	Write_com(0x0C);	//显示器开关控制：开显示，光标不显示、不闪烁
	Write_com(0x06);	//设置输入模式：AC自动加1，显示不移位
	Write_com(0x01);	//清屏
}
/**************************************/

/***************定时器0_初始化***************/
void Timer0_Init(void)
{
	TMOD = 0x01;   	//装初值
	TH0 = 0xD8;		//10ms
	TL0 = 0xF0;
	ET0 = 1;
	EA = 1;	
	TR0 = 1;
}

//定时器0_中断服务函数。计时、刷新显示
void Timer0_ISR(void) interrupt 1	
{
	static uchar i = 0;
	i++;	//10ms 加1	
	TH0 = 0xD8;		//重装初值
	TL0 = 0xF0;		//10ms
	if(100 == i)		//1S时间到了
	{
		i = 0;		
		Timer();	//1s进入一次，计算秒、分、时		
	}
	Refresh_Display();	//刷新显示
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