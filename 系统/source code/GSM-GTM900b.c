#include<STC12C5A.h>
#include"MYTYPE.h"
#include"GSM-GTM900b.h"
#include"delay.h"


uchar TEMP[80] = {0};
uchar CSQ[3] = {0,0,'\0'};	// 存放信号质量值
uchar gsm_delay_time = 0;	// GSM模块等待初始化时间
uchar code GSM_TAB[] = {"当前信号："};	// 当前信号： 

sbit LED = P1^7;

bit GSM_tsim_ok = 0;	// SIM卡是否在位标识
bit GSM_cmgf_ok = 0;	// 文本设置完成标识
bit GSM_csmp_ok = 0;	// SMS格式设置完成标识
bit GSM_csq_ok = 0;		// 信号强度获取标识
bit GSM_cmgs_ok = 0;   	// 电话号码发送完成标识
bit GSM_sms_ready_ok = 0;
bit GSM_receive_ok = 0;	// “接收到短信”标识
bit GSM_cops_ok = 0;	// 运营商获取成功标识
bit GSM_cnmi_ok = 0;	// 短信提示设置完成标识

bit GSM_CMCC = 0;
bit GSM_CUCC = 0;

uchar COUNT = 0;
uchar SIZE = 0;		// 接收GSM传回数据长度


void GSM_RX_clear(void)
{
	uchar i = 0;
	for(i = 0; i < SIZE; i++)
		TEMP[i] = 0x01;
}

void GSM_sms_ready(uchar value)
{	// 头、短信中心号码、中、目标号码、尾+内容长度、Unicode内容

	uchar *psend_buffer;
	uchar send_len;
	uchar i;
	// unicode
	uchar code gas[] = "8B66544AFF1A53D173B067095BB36C144F53";// 警告：发现有害气体
	uchar code temperature[] = "8B66544AFF1A6E295EA68FC79AD8";// 警告：温度过高

//	uchar code TEL[] = {0x22,0x31,0x33,0x30,0x32,0x35,0x31,0x30,0x30,0x30,0x34,0x36,0x22,0x0D};	// "13025100046"回车
//	uchar code TEL[] = {0x22,0x31,0x33,0x32,0x33,0x32,0x37,0x34,0x30,0x36,0x30,0x38,0x22,0x0D,0x0a};	// "13232740608"回车 换行
	uchar code TEL[] = {0x22,0x31,0x33,0x31,0x32,0x39,0x34,0x33,0x33,0x30,0x33,0x37,0x22,0x0D,0x0a};	// "13129433037"回车 换行
//	uchar code TEL[] = {0x22,0x31,0x35,0x35,0x31,0x32,0x36,0x38,0x34,0x39,0x33,0x37,0x22,0x0D,0x0a};	// "15512684937"回车 换行

	//uchar code TAB3[] = "62A58B666E295EA68BBE7F6E5B8C6BD5";			// 报警温度设置完毕
	//uchar code TAB4[] = "7EE7753556685DF262535F00";					// 继电器已打开
	//
	//uchar code TAB5[] = "62535F007EE775355668";				// 打开继电器
	//uchar code TAB6[] = "517395ED7EE775355668";				// 关闭继电器
	//uchar code TAB7[] = "83B753D65F53524D6E295EA6";			// 获取当前温度
	//uchar code TAB8[] = "8BBE7F6E62A58B666E295EA6FF1A";		// 设置报警温度：
	switch(value)
	{
		case warning_gas:
		{
			psend_buffer = gas;
			send_len = 36;
		}
		break;
		case warning_t:
		{
			psend_buffer = temperature;
			send_len = 76;				
		}
		break;
//		case open_relay:
//		{}
//		break;
//		case close_relay:
//		{}
//		break;
		default:
		break;
	}
	GSM_TX(GSM_CSMP);	// 
	GSM_RX(GSM_CSMP);
	DelayMs(100);
	GSM_TX(GSM_CMGF);	// 设置文本模式
	GSM_RX(GSM_CMGF);
	DelayMs(100);
	GSM_TX(GSM_CMGS);	// 设置电话号码
	for(i = 0; i < 15; i++)		// 发送电话号码
	{
		SBUF = TEL[i];
		while(!TI);
		TI = 0;
	}
	GSM_RX(GSM_CMGS);
	DelayMs(100);			
	for(i = 0; i < send_len; i++)		// 发送PDU码
	{
		SBUF = *(psend_buffer+i);
		while(!TI);
		TI = 0;
	}
	DelayMs(100);
	SBUF = 0x1a;						// 发送0x1A
	while(!TI);
	TI = 0;
	// 读取是否ok
}

void GSM_RX(uchar value)	// 命令发送后的接收
{
	uchar tmp;
	uint amount = 500;
	switch(value)
	{
		case GSM_CMGF:		// 设置文本模式
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// 串口接收到的内容装进TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// 接收到K、回车、换行
						{
							SIZE = COUNT;	// 保存字符串串长度
							COUNT = 0;
							GSM_cmgf_ok = 1;
							break;									
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// 接收到R、回车、换行
						{
							COUNT = 0;
							GSM_cmgf_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// 保险
						{
							COUNT = 0;
							break;												
						}						
					}	
					RI = 0;
				}			
			}			
		} 
		break;
		case GSM_CSMP:	// 设置sms格式
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// 串口接收到的内容装进TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// 接收到K、回车、换行
						{
							SIZE = COUNT;	// 保存字符串串长度
							COUNT = 0;
							GSM_csmp_ok = 1;
							break;		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// 接收到R、回车、换行
						{
							COUNT = 0;
							GSM_csmp_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// 保险
						{
							COUNT = 0;
							break;												
						}						
					}	
					RI = 0;
				}			
			}		
		}
		break;
		case GSM_CMGS:	// 号码发送后
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// 串口接收到的内容装进TEMP
						if(0x20 == TEMP[COUNT] && 0x3e == TEMP[COUNT-1])		// 接收到"> "
						{
							SIZE = COUNT;	// 保存字符串串长度
							COUNT = 0;
							GSM_cmgs_ok = 1;  // 号码发送完成
							break;		// 跳出									
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// 接收到R、回车、换行
						{
							COUNT = 0;
							GSM_cmgs_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// 保险
						{
							COUNT = 0;
							break;												
						}						
					}	
					RI = 0;
				}			
			}		
		}
		break;
		case GSM_TSIM:	// SIM卡检测
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// 串口接收到的内容装进TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// 接收到K、回车、换行
						{
							SIZE = COUNT;	// 保存字符串串长度
							COUNT = 0;
							break;			// 接收完成，跳出		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// 接收到R、回车、换行
						{
							COUNT = 0;
							GSM_tsim_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// 保险
						{
							COUNT = 0;
							break;												
						}						
					}	
					RI = 0;
				}			
			}		
		}
		break;
		case GSM_COPS:	// 运营商检测
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// 串口接收到的内容装进TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// 接收到K、回车、换行
						{
							SIZE = COUNT;	// 保存字符串串长度
							COUNT = 0;
							break;			// 接收完成，跳出		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// 接收到R、回车、换行
						{
							COUNT = 0;
							GSM_cops_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// 保险
						{
							COUNT = 0;
							break;												
						}						
					}	
					RI = 0;
				}			
			}		
		}
		break;
		case GSM_CSQ:	// 信号检测
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// 串口接收到的内容装进TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// 接收到K、回车、换行
						{
							SIZE = COUNT;	// 保存字符串串长度
							COUNT = 0;
							break;			// 接收完成，跳出		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// 接收到R、回车、换行
						{
							COUNT = 0;
							GSM_csq_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// 保险
						{
							COUNT = 0;
							break;												
						}						
					}	
					RI = 0;
				}			
			}		
		}
		break;
		case GSM_CNMI:	// 设置短信接收提醒
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// 串口接收到的内容装进TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// 接收到K、回车、换行
						{
							SIZE = COUNT;	// 保存字符串串长度
							COUNT = 0;
							GSM_cnmi_ok = 1;	// 短信接收提醒设置完成
							break;			// 完成，跳出		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// 接收到R、回车、换行
						{
							COUNT = 0;
							GSM_cnmi_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// 保险
						{
							COUNT = 0;
							break;												
						}						
					}	
					RI = 0;
				}			
			}		
		}
		break;
		default:
		break;
	}
}


void GSM_TX(uchar value)	// 串口发送GSM AT指令
{
	uchar len,i;
	uchar *pfunction;
	//				  长度
	uchar code TSIM[] = {8,0x41,0x54,0x25,0x54,0x53,0x49,0x4D,0x0D}; 						// AT%TSIM回车
	uchar code CSQ[] = {7,0x41,0x54,0x2B,0x43,0x53,0x51,0x0D}; 								// AT+CSQ回车
	uchar code CMGF[] = {10,0x41,0x54,0x2B,0x43,0x4D,0x47,0x46,0x3D,0x31,0x0D}; 			// AT+CMGF=x回车
	uchar code CMGS[] = {8,0x41,0x54,0x2B,0x43,0x4D,0x47,0x53,0x3D}; 					// AT+CMGS=
	uchar code CNMI[] = {12,0x41,0x54,0x2B,0x43,0x4E,0x4D,0x49,0x3D,0x32,0x2C,0x31,0x0D}; 	// AT+CNMI=2,1回车
	uchar code CMGR[] = {10,0x41,0x54,0x2B,0x43,0x4D,0x47,0x52,0x3D,0x78,0x0D}; 			// AT+CMGR=x回车
	uchar code COPS[] = {9,0x41,0x54,0x2B,0x43,0x4F,0x50,0x53,0x3F,0x0D}; 			// AT+COPS?回车
	uchar code CSMP[] = {19,0x41,0x54,0x2B,0x43,0x53,0x4D,0x50,0x3D,0x31,0x37,0x2C,0x31,0x36,0x37,0x2C,0x30,0x2C,0x38,0x0D};// 发送前设置	
	//AT+CSMP=17,167,0,8
	switch(value)
	{
		case GSM_TSIM:	// 检测卡
		{
			len = TSIM[0];
			pfunction = TSIM;
		}		
		break;
		case GSM_CSQ:	// 检测信号
		{
			len = CSQ[0];
			pfunction = CSQ;		
		}
		break;
		case GSM_CMGF:	// 设置文本模式
		{
			len = CMGF[0];
			pfunction = CMGF;		
		}
		break;
		case GSM_CMGS:	// 输入电话号码 前
		{
			len = CMGS[0];
			pfunction = CMGS;		
		}
		break;
		case GSM_CNMI:	// 设置短信提醒
		{
			len = CNMI[0];
			pfunction = CNMI;		
		}
		break;
		case GSM_CMGR:	// 短信读取
		{
			len = CMGR[0];		
			pfunction = CMGR;
		}		
		break;
		case GSM_CSMP:	// 设置短信格式
		{
			len = CSMP[0];		
			pfunction = CSMP;
		}
		break;
		case GSM_COPS:	// 查询运营商
		{
			len = COPS[0];		
			pfunction = COPS;
		}
		break;
		default:
		break; 		
	}
	for(i = 0; i < len; i++)
	{
		SBUF = *(pfunction+1+i);
		while(!TI);
		TI = 0;
	}
}

void GSM_TSIM_manage(void)
{
	if('1' == TEMP[SIZE-8])
	{
		GSM_tsim_ok = 1;	// SIM卡在位
	}
	else if('0' == TEMP[SIZE-8])
	{
		GSM_tsim_ok = 0;	// SIM卡不在位
	}					
}

void GSM_CSQ_manage(void)
{
	CSQ[1] = TEMP[SIZE-12];		// 信号个位
	CSQ[0] = TEMP[SIZE-13];		// 信号十位
//	if(CSQ[1] != '9')
		GSM_csq_ok = 1;
}

void GSM_COPS_manage(void)
{
	if(0x22 == TEMP[SIZE-8] && 0x4D == TEMP[SIZE-9])	// (M") 联通
	{
		GSM_CUCC = 1;
		GSM_cops_ok = 1;	// 运营商检测完成	
	}
	else if('0' == TEMP[SIZE-8])	// 未获取运营商
	{
		GSM_CUCC = 0;	
		GSM_CMCC = 0;
	}
//	else if(0x22 == TEMP[SIZE-8] && 0x4D == TEMP[SIZE-9])	// (*") 移动
//	{
//	GSM_CMCC = 1;			
//	GSM_cops_ok = 1;	// 运营商检测完成	
//	}	
}

void GSM_Init(void)
{
	uchar i;	
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_TSIM);		// 发送检测SIM卡
		GSM_RX(GSM_TSIM);		// 接收GSM模块传回数据
		DelayMs(100);
		GSM_TSIM_manage();		// 判断接收数据
		GSM_RX_clear();			// 清空接收数据
		if(GSM_tsim_ok)
			break;		// 检测到SIM卡提前跳出				
	}
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_COPS);		// 发送检测运营商
		GSM_RX(GSM_COPS);		// 接收GSM模块传回数据
		DelayMs(100);
		GSM_COPS_manage();		// 判断接收数据
		GSM_RX_clear();			// 清空接收数据
		if(GSM_cops_ok)
			break;		// 检测到运营商提前跳出						
	}
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_CSQ);		// 发送检测信号
		GSM_RX(GSM_CSQ);		// 接收GSM模块传回数据
		DelayMs(100);
		GSM_CSQ_manage();		// 判断接收数据
		GSM_RX_clear();			// 清空接收数据
		if(GSM_csq_ok)
			break;		// 检测到信号提前跳出	
	}	
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_CNMI);		// 发送设置短信提醒
		GSM_RX(GSM_CNMI);		// 接收GSM模块传回数据
		DelayMs(100);
		GSM_RX_clear();			// 清空接收数据
		if(GSM_cnmi_ok)			
			break;		// 设置完成 提前跳出				
	}	
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_CSMP);		// 发送设置SMS格式
		GSM_RX(GSM_CSMP);		// 接收GSM模块传回数据
		DelayMs(100);
		GSM_RX_clear();			// 清空接收数据
		if(GSM_csmp_ok)
			break;	// 设置完成 提前跳出				
	}
}

/*  11.0592Mhz	Uart串口初始化		    */
void Uart_Init(void) 	// 串口初始化
{
	TMOD &= 0x0F;	// 保留定时器0设置，清空定时器1设置
	TMOD |= 0x20;	// 定时器1 模式2
	SCON |= 0x50;	// 串口方式1，8位UART波特率可变，可接收
	TH1   = 0xFD;	// TL1溢出后将TH1的值装入
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// 关闭定时器1中断
//	ES    = 1;		// 打开串口中断
//	EA    = 1;	    // 打开总中断
	TR1   = 1;		// 开定时器1
}
