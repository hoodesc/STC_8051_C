#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int
uint Num;	//全局变量Num，存储按键+，-后计算出来的定时器初值
uint Init_data = 5;
typedef enum Key1_State{InitState,AffirmState,SingleState,RepeatState};
typedef enum Key2_State{InitState2,AffirmState2,SingleState2,RepeatState2};

sbit PWM = P1^7;
sbit KEY1 = P1^6;
sbit KEY2 = P1^5;


void Timer0_Init(void)	    //按键检测_定时器0
{
	TMOD = 0x01;	
	TH0 = 0xD8;		//10ms
	TL0 = 0xF0;
	TR0 = 1;
}

void Timer1_Init(void)		//高低电平切换_定时器1
{
	TMOD = 0x10;	
	TH1 = 0xD8;	//10ms扫描一次
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
			value = 1;					//状态值设置为1，下次将进入高电平
			TH1 = (65536-46636+Num)/256;		//获取按键扫描值Num来计算初值
			TL1 = (65536-46636+Num)%256;
			PWM = 0;
		break;
		case 1:
			value = 0; 					//状态值设置为0，下次将进入低电平
			TH1 = (65536-Num)/256;;		//获取按键扫描值Num来计算初值
			TL1 = (65536-Num)%256;;
			PWM = 1;
		break;	
	}
}

uchar Key1_scan()		//按键1扫描
{
	static uchar Time_1 = 0;		//时间标志，1次=10ms
	uchar KeyStateValue = 0;
	uchar Key1_Value;	//按键1状态标志
	Key1_Value = KEY1;


	switch (KeyStateValue)
	{
		case InitState:							//初始化状态
			if(0 == Key1_Value)			//按键被按下
			{
				KeyStateValue = AffirmState;	//下次_进入确认状态
			}
			else {KeyStateValue = InitState;}
		break;
		case AffirmState:						//确认状态
			if(0 == Key1_Value)
			{
				KeyStateValue = SingleState;	//下次_进入单次触发状态	
			}
			else {KeyStateValue = InitState;}
		break;
		case SingleState:						//单次触发状态
			Time_1++;
			if(Time_1 > 100)		////如果按下时间大于1S
			{
				Time_1 = 0;
			 	KeyStateValue = InitState;		//下次_进入连发状态
				
			}
			else if(1 == Key1_Value)
			{
			 	Init_data++;
			}
		break;
		case RepeatState:						//连发状态
			Time_1++;
			if(1 == Key1_Value)					//如果按键释放
			{
			 	KeyStateValue = InitState;		//下次_进入初始化状态
			}
			if(Time_1 > 10)		//如果按下时间大于100ms
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


uchar Key2_scan()		//按键2扫描
{
	static uchar Time_2 = 0;		//时间标志，1次=10ms
	uchar KeyStateValue = 0;
	uchar Key2_Value;
	Key2_Value = KEY2;

	switch (KeyStateValue)
	{
		case InitState2:							//初始化状态
			if(Key2_Value == 0)			//按键被按下
			{
				KeyStateValue = AffirmState2;	//下次_进入确认状态
			}
			else {KeyStateValue = InitState2;}
		break;
		case AffirmState2:						//确认状态
			if(Key2_Value == 0)
			{
				KeyStateValue = SingleState2;	//下次_进入单次触发状态	
			}
			else {KeyStateValue = InitState2;}
		break;
		case SingleState2:						//单次触发状态
			Time_2++;
			if(Time_2 > 100)		////如果按下时间大于1S
			{
				Time_2 = 0;
				KeyStateValue = RepeatState2;	//下次_进入连发状态
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
		case RepeatState2:						//连发状态
			Time_2++;
			if(1 == Key2_Value)					//如果按键释放
			{
			 	KeyStateValue = InitState2;		//下次_进入初始化状态
			}
			if(Time_2>10)							//如果按下时间大于100ms
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
		if(1 == TF0)	//定时器0_溢出标志位
		{
			TF0 = 0;	//清零
			if(0 == KEY1)
			{
			 	Key1_scan();
				Num =Key1_scan()*100;		//加
			}
			else if(0 == KEY2)
			{
			    Key2_scan();
				Num = Key2_scan()*100;		//减
			}
		}
	}
 	
}


