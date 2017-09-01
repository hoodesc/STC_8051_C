#include <reg52.h>
//LCD1602 两个按键设置时间
//晶振：12Mhz
//MCU:STC12c4052ad
//长按按键1进入设置模式，短按时间加1
//长按按键2在设置模式下切换S、M、H位，短按时间减1
typedef unsigned char uchar;
typedef unsigned int uint;

uchar H_ge,H_shi = 0;
uchar M_ge,M_shi = 0;
uchar S_ge,S_shi = 0;
uchar H,M,S = 0;					//时分秒
uchar Time_Set_Value = 0;		//设置模式下，时、分、秒选选择标志；	0、秒S		1、分M		2、时H
uchar Set_StateValue = 0;		//正常、设置模式标志；		0、正常模式		1、设置模式
uchar code Num[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39}; 	//0-9  十六进制数
uchar code C_TIME[]="TIME: ";
uchar code C_SET[]="SET ";
uchar code C_ADD[]="ADD";
uchar code C_SUB[]="SUB";
typedef enum Key1_State{Init1_State,Affirm1_State,Single1_State,Repeat1_State};
typedef enum Key2_State{Init2_State,Affirm2_State,Single2_State,Repeat2_State};

//位定义
sbit RS = P3^5;		//寄存器选择		1、数据		0、指令
sbit RW = P3^4;		//读写操作选择	1、读			0、写
sbit EN = P3^3;		//使能信号输入端	低电平有效
sbit KEY1 = P3^2;	//按键1
sbit KEY2 = P3^7;	//按键2

//函数声明
void DelayMs(uint a);								//延时函数
void Timer_Work(void);							//正常模式时间1秒+1
void Timer_Work_Sub(uchar i);				//SUB标志
void Timer_Work_Add(uchar i);				//ADD标志
void Write_Data(uchar d);						//写数据
void Write_Instructions(uchar c);		//写指令
void Display_Refresh(uchar i);			//显示刷新
void Key1Scan(void);								//按键1扫描
void Key2Scan(void);								//按键2扫描
void Set_Function(void);						//设置模式
void LCD_Init(void);								//LCD初始化
void Add_SUB_Value(uchar i);				//选择时、分、秒 标志显示

//延时函数
void DelayMs(uint a){
	uint j;
	while( a-- != 0){
		for(j = 0; j < 125; j++);
	}
}

//写数据
void Write_Data(uchar d)	
{
	RS = 1;
	RW = 0;
	EN = 1;
	DelayMs(1);
	P1 = d;
	EN = 0;
}
//写指令
void Write_Instructions(uchar c)	
{
	RS = 0;
	RW = 0;
	EN = 1;
	DelayMs(1);
	P1 = c;
	EN = 0;
}


//正常模式下，每秒+1
void Timer_Work(void)		
{
	S++;				//1S加一次
	if(60 == S)
	{
		S = 0;
		M++;
	}
	if(60 == M)
	{
		M = 0;
		H++;
	}
	if(24 == H)
	{
		H = 0;
	}
	S_ge = S%10;	S_shi = S/10;
	M_ge = M%10;	M_shi = M/10;
	H_ge = H%10;	H_shi = H/10;
}
//时、分、秒 减
void Timer_Work_Sub(uchar i)		
{
	switch(i)
	{
		case 0:
		{
			if(S != 0)
			{
				S--;
			}	
		}
		break;
		case 1:
		{
			if(M != 0)
			{
				M--;
			}
		}
		break;
		case 2:
		{
			if(H != 0)
			{
				H--;
			}
		}
		break;
	}
	S_ge = S%10;	S_shi = S/10;
	M_ge = M%10;	M_shi = M/10;
	H_ge = H%10;	H_shi = H/10;
}
//时、分、秒	加
void Timer_Work_Add(uchar i)		
{
	switch(i)
	{
		case 0:
		{
			if(S == 59)
			{S = 0;}
			else{S++;}
		}
			break;
		case 1:
		{
			if(M == 59)
			{M = 0;}
			else{M++;}
		}
			break;
		case 2:
		{
			if(H == 23)
			{H = 0;}
			else{H++;}
			break;
		}
	}
	S_ge = S%10;	S_shi = S/10;
	M_ge = M%10;	M_shi = M/10;
	H_ge = H%10;	H_shi = H/10;
}

//选择时、分、秒 标志显示
void Add_SUB_Value(uchar i)
{
	uchar add,sub,cls = 0;
	Write_Instructions(0x80+0x4A);
	switch(i)
	{
		case 0:
		{
			if(0 == Time_Set_Value)		//设置模式下，时、分、秒选选择标志；	0、秒S		1、分M		2、时H
			{
				Write_Data(0x28);	Write_Data(0x53); Write_Data(0x29);		//写入:(S)
			}
			else if(1 == Time_Set_Value)		//1、分
			{
				Write_Data(0x28);	Write_Data(0x4D); Write_Data(0x29);		//写入:(M)
			}
			else{Write_Data(0x28);	Write_Data(0x48); Write_Data(0x29);}		//2、时;写入:(H)
			for(sub=0;sub<=3;sub++)
			{
				Write_Data(C_SUB[sub]);
			}	
		}
			break;
		case 1:
		{
			if(0 == Time_Set_Value)		//设置模式下，时、分、秒选选择标志；	0、秒S		1、分M		2、时H
			{
				Write_Data(0x28);	Write_Data(0x53); Write_Data(0x29);		//写入:(S)
			}
			else if(1 == Time_Set_Value)		//1、分
			{
				Write_Data(0x28);	Write_Data(0x4D); Write_Data(0x29);		//写入:(M)
			}
			else{Write_Data(0x28);	Write_Data(0x48); Write_Data(0x29);}		//2、时;写入:(H)		
			for(add=0;add<=3;add++)
			{
				Write_Data(C_ADD[add]);
			}	
		}
			break;
		case 2:				//清零
		{
			Write_Instructions(0x80+0x4A);
			for(cls=0;cls<=6;cls++)
			{
				Write_Data(0x20);
			}	
		}
			break;
	}
}

//定时器0初始化
void Timer0_Init()
{
	TMOD = 0x01;
	TH0 = 0xD8;
	TL0 = 0xF0;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}
//定时器0中断服务函数
void Timer0_ISR(void) interrupt 1
{
	static uchar i = 0;
	i++;
	TH0 = 0xD8;
	TL0 = 0xF0;
	if(100 == i)		//调试。正常100
	{
		i = 0;
		Timer_Work();
	}
	Display_Refresh(0);
}

//进入设置模式后，“TIME”改写为“SET ”
void Set_Function(void)
{
	uchar i = 0;
	Write_Instructions(0x80);
	for(i=0;i<4;i++)
	{
		Write_Data(C_SET[i]);
	}
}

//按键1扫描
void Key1Scan()
{
	static uint time = 0;
	static uchar Key1_StateValue = 0;
	static uchar KEY1_Value = 0;
	KEY1_Value = KEY1;		//获取按键1状态
	switch (Key1_StateValue)
	{
		case Init1_State:		//初始化状态
		{
			if(!KEY1_Value)
			{	
				Key1_StateValue = Affirm1_State;		//消抖，按键按下，下次进入确认状态
			}
			else{Key1_StateValue = Init1_State;}	//抖动，按键放开，下次进入初始化状态
		}
		break;
		case Affirm1_State:		//确认状态
		{
			if(!KEY1_Value)
			{
				time = 0;
				Key1_StateValue = Single1_State;		//稳定，按键按下，下次进入单次触发状态
			}
			else{Key1_StateValue = Init1_State;}	//不稳定，按键放开，下次进入初始化状态
		}
		break;
		case Single1_State:		//单次触发状态
		{
			time++;
			if(KEY1)		//如果按键1放开，则触发一次
			{
				if(Set_StateValue)		//判断是否在 设置模式
				{
					Timer_Work_Add(Time_Set_Value);
					Display_Refresh(1);			//显示刷新，且不显示“TIME”
					Add_SUB_Value(1);				//第二行显示“ADD”标志
				}
				Key1_StateValue = Init1_State;
			}
			else if(200 == time)
			{
					time = 0;
					Add_SUB_Value(2);			//第二行，字符用“空格”替换
					Key1_StateValue = Repeat1_State;	//按键按下时间等于2s，下次进入长按状态
			}			
		}
		break;
		case Repeat1_State:		//长按状态
		{
				TR0 = ~TR0;
				if(!TR0)
				{
					Set_Function();				//进入设置模式
					Set_StateValue = 1;		//正常、设置模式标志；	0、正常模式		1、设置模式
					Time_Set_Value  = 0;	//设置模式下，时、分、秒选选择标志；	0、秒S		1、分M		2、时H
					Write_Instructions(0x80+0x4A);
					Write_Data(0x28);	Write_Data(0x53); Write_Data(0x29);		//第二行写入：(S)
					while(1)		//长按进入设置模式后，放开按键，S不加1
					{
						if(KEY1)
						{break;}
					}
				}
				else
				{
					Set_StateValue = 0;		////正常、设置模式标志；	0、正常模式		1、设置模式
				}
			Key1_StateValue = Init1_State;
		}
		break;
		default:Key1_StateValue = Init1_State;
	}
}

//按键2扫描
void Key2Scan()
{
	static uint time = 0;
	static uchar Key2_StateValue = 0;		//按键2状态标志
	static uchar KEY2_Value = 0;				//按键2标志
	KEY2_Value = KEY2;		//获取按键状态
	switch (Key2_StateValue)
	{
		case Init2_State:		//初始化状态
		{
			if(!KEY2_Value)
			{	
				Key2_StateValue = Affirm2_State;		//消抖，按键按下，下次进入确认状态
			}
			else{Key2_StateValue = Init2_State;}	//抖动，按键放开，下次进入初始化状态
		}
		break;
		case Affirm2_State:		//确认状态
		{
			if(!KEY2_Value)
			{
				time = 0;
				Key2_StateValue = Single2_State;		//稳定，按键按下，下次进入单次触发状态
			}
			else{Key2_StateValue = Init2_State;}	//不稳定，按键放开，下次进入初始化状态
		}
		break;
		case Single2_State:		//单次触发状态
		{
			time++;
			if(KEY2)
			{
				if(Set_StateValue)
				{
					Timer_Work_Sub(Time_Set_Value);
					Display_Refresh(1);			//显示刷新，且不显示“TIME”
					Add_SUB_Value(0);				//第二行显示“SUB”标志
					Key2_StateValue = Init2_State;	
				}
				else{Key2_StateValue = Init2_State;}	//防止正常模式下，短按按键2 后立刻 长按按键1 导致放开按键1会切换到"M"
			}
			else if(150 == time)		
			{
				time = 0;
				Key2_StateValue = Repeat2_State;	//按下时间等于1.5秒，下次进入长按模式
			}
		}
		break;
		case Repeat2_State:		//长按状态
		{
			if(Set_StateValue)
			{
				Time_Set_Value++;
				Write_Instructions(0x80+0x4A);
				if(3 == Time_Set_Value)
				{
					Time_Set_Value = 0;
				}				
				if(0 == Time_Set_Value)
				{
					Add_SUB_Value(2); Write_Instructions(0x80+0x4A);
					Write_Data(0x28);	Write_Data(0x53); Write_Data(0x29);
				}
				else if(1 == Time_Set_Value)
				{
					Add_SUB_Value(2); Write_Instructions(0x80+0x4A);
					Write_Data(0x28);	Write_Data(0x4D); Write_Data(0x29);
				}
				else{Add_SUB_Value(2); Write_Instructions(0x80+0x4A); Write_Data(0x28);	Write_Data(0x48); Write_Data(0x29);}
				
				while(1)		//长按进入设置模式后，放开按键，S不加1
				{
					if(KEY2){break;}
				}
			Key2_StateValue = Init2_State;		//下次进入初始化模式
			}
			else{Key2_StateValue = Init2_State;}	//防止正常模式下，长按按键2松开 后立刻 长按按键1 导致放开按键1会切换到"M"
		}
		break;
		default:Key2_StateValue = Init2_State;		
	}
}

//LCD初始化
void LCD_Init(void)
{
	uchar i = 0;
	DelayMs(1);
	Write_Instructions(0x80);
	DelayMs(5);
	for(i=0;i<5;i++)
	{
		Write_Data(C_TIME[i]);
	}
}

//显示刷新
void Display_Refresh(uchar i)
{
	if(!i)
	{
		LCD_Init();
	}
	Write_Instructions(0x86);
	Write_Data(Num[H_shi]);Write_Data(Num[H_ge]);
	Write_Data(0x3A);
	Write_Data(Num[M_shi]);Write_Data(Num[M_ge]);
	Write_Data(0x3A);
	Write_Data(Num[S_shi]);Write_Data(Num[S_ge]);
}

void main()
{
	Write_Instructions(0x38);		//8位数据总线、2行显示、5*8
	Write_Instructions(0x0C);		//显示开，光标不显示、不闪烁
	Write_Instructions(0x06);		//完成一个字符码传送后AC自动加1、显示不移位
	Write_Instructions(0x01);		//清屏
	//Display_Init();
	Timer0_Init();
	while(1)
	{
		Key1Scan();
		DelayMs(10);
		Key2Scan();
		DelayMs(10);
	}
}