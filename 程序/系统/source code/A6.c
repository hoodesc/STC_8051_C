#include "STC15.H"
#include "mytype.h"
#include "delay.h"
#include "string.h"
#include "a6.h"
#include "uart.h"

uchar code mq2_normal[] = "mq2:normal";
uchar code mq2_warning[] = "mq2:warning";
uchar code huoyan_warning[] = "huoyan:warning";
uchar code huoyan_normal[] = "huoyan:normal";

//串口1发送回车换行
#define UART1_SendLR() UART1_SendData(0X0D);\
											 UART1_SendData(0X0A)

/*************  本地变量声明	**************/
uchar Uart1_Buf[Buf1_Max];
uchar Register_Flag=0;  //A6  注册标志位  1 ：注册成功  3：注册失败   5：注册成功，但处于漫游状态下，无法拨打电话发短信
uchar First_Int = 0;


//短信接收方手机号
static unsigned char *phone="\"13129433037\"";     


void CLR_Buf1(void)
{
	uint k;
	for(k=0;k<Buf1_Max;k++)      //将缓存内容清零
	{
		Uart1_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}

uchar Find(uchar *a)
{ 
  if(strstr(Uart1_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

void Second_AT_Command(uchar *b,uchar *a,uchar wait_time)         
{
	uchar i;
	uchar *c;
	c = b;										//保存字符串地址到c
	CLR_Buf1(); 
  i = 0;
	while(i == 0)                    
	{
		
		if(!Find(a))            //查找需要应答的字符
		{
				b = c;							//将字符串地址给b
				for (b; *b!='\0';b++)
				{
					UART1_SendData(*b);
				}
				UART1_SendLR();	
				DelayMs(100 * wait_time);
    }
 	  else
		{
			i = 1;
		}
	}
	CLR_Buf1(); 
}

void Set_Text_Mode(void)
{
	uchar buf[30]="AT+CSCS="; 
//	Second_AT_Command("ATE0","OK",3);										//取消回显	
	Second_AT_Command("AT+CMGF=1","OK",3);				  //配置短信为TEXT模式
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//所有操作都在SIM卡中进行
	strcat(buf,"\"GSM\""); //字符串拼接函数（库函数）
	Second_AT_Command(buf,"OK",3);//设置输入字符集为"GSM"格式 
}

void Send_Message(uchar *padt)
{
	uchar buf[30]="AT+CMGS="; 
	strcat(buf,phone); //字符串拼接函数（库函数）
//	Second_AT_Command(buf,">",5);//发送指令后直到出现 ">"  后发送内容
	UART1_SendString(buf);
	UART1_SendLR();
	DelayMs(50);
	UART1_SendString(padt);
	UART1_SendData(0x1a);
}

void Wait_CREG(void)
{
	uchar i;
	uchar k;
	i = 0;
	CLR_Buf1();
  while(i == 0)        			
	{
    
		CLR_Buf1();        
		UART1_SendString("AT+CREG?");
		UART1_SendLR();
		DelayMs(500); 
		
	    for(k=0;k<Buf1_Max;k++)      			
    	{
			if((Uart1_Buf[k] == 'C')&&(Uart1_Buf[k+1] == 'R')&&(Uart1_Buf[k+2] == 'E')&&(Uart1_Buf[k+3] == 'G')&&(Uart1_Buf[k+4] == ':'))
			{
					 
				if((Uart1_Buf[k+8] == '1')&&(Uart1_Buf[k+9] != '3'))
				{
					i = 1;
					Register_Flag=1;
				  break;
				}
        if((Uart1_Buf[k+8] == '1')&&(Uart1_Buf[k+9] == '3')) 
				{
					i = 1;
					Register_Flag=3;
				  break;
				}	
			  if(Uart1_Buf[k+8] == '5') 
				{
					i = 1;
					Register_Flag=5;
				  break;
				}				
			}
		}		
	}
}