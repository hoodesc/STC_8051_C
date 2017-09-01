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

//����1���ͻس�����
#define UART1_SendLR() UART1_SendData(0X0D);\
											 UART1_SendData(0X0A)

/*************  ���ر�������	**************/
uchar Uart1_Buf[Buf1_Max];
uchar Register_Flag=0;  //A6  ע���־λ  1 ��ע��ɹ�  3��ע��ʧ��   5��ע��ɹ�������������״̬�£��޷�����绰������
uchar First_Int = 0;


//���Ž��շ��ֻ���
static unsigned char *phone="\"13129433037\"";     


void CLR_Buf1(void)
{
	uint k;
	for(k=0;k<Buf1_Max;k++)      //��������������
	{
		Uart1_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
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
	c = b;										//�����ַ�����ַ��c
	CLR_Buf1(); 
  i = 0;
	while(i == 0)                    
	{
		
		if(!Find(a))            //������ҪӦ����ַ�
		{
				b = c;							//���ַ�����ַ��b
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
//	Second_AT_Command("ATE0","OK",3);										//ȡ������	
	Second_AT_Command("AT+CMGF=1","OK",3);				  //���ö���ΪTEXTģʽ
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//���в�������SIM���н���
	strcat(buf,"\"GSM\""); //�ַ���ƴ�Ӻ������⺯����
	Second_AT_Command(buf,"OK",3);//���������ַ���Ϊ"GSM"��ʽ 
}

void Send_Message(uchar *padt)
{
	uchar buf[30]="AT+CMGS="; 
	strcat(buf,phone); //�ַ���ƴ�Ӻ������⺯����
//	Second_AT_Command(buf,">",5);//����ָ���ֱ������ ">"  ��������
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