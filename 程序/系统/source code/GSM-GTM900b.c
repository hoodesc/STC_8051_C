#include<STC12C5A.h>
#include"MYTYPE.h"
#include"GSM-GTM900b.h"
#include"delay.h"


uchar TEMP[80] = {0};
uchar CSQ[3] = {0,0,'\0'};	// ����ź�����ֵ
uchar gsm_delay_time = 0;	// GSMģ��ȴ���ʼ��ʱ��
uchar code GSM_TAB[] = {"��ǰ�źţ�"};	// ��ǰ�źţ� 

sbit LED = P1^7;

bit GSM_tsim_ok = 0;	// SIM���Ƿ���λ��ʶ
bit GSM_cmgf_ok = 0;	// �ı�������ɱ�ʶ
bit GSM_csmp_ok = 0;	// SMS��ʽ������ɱ�ʶ
bit GSM_csq_ok = 0;		// �ź�ǿ�Ȼ�ȡ��ʶ
bit GSM_cmgs_ok = 0;   	// �绰���뷢����ɱ�ʶ
bit GSM_sms_ready_ok = 0;
bit GSM_receive_ok = 0;	// �����յ����š���ʶ
bit GSM_cops_ok = 0;	// ��Ӫ�̻�ȡ�ɹ���ʶ
bit GSM_cnmi_ok = 0;	// ������ʾ������ɱ�ʶ

bit GSM_CMCC = 0;
bit GSM_CUCC = 0;

uchar COUNT = 0;
uchar SIZE = 0;		// ����GSM�������ݳ���


void GSM_RX_clear(void)
{
	uchar i = 0;
	for(i = 0; i < SIZE; i++)
		TEMP[i] = 0x01;
}

void GSM_sms_ready(uchar value)
{	// ͷ���������ĺ��롢�С�Ŀ����롢β+���ݳ��ȡ�Unicode����

	uchar *psend_buffer;
	uchar send_len;
	uchar i;
	// unicode
	uchar code gas[] = "8B66544AFF1A53D173B067095BB36C144F53";// ���棺�����к�����
	uchar code temperature[] = "8B66544AFF1A6E295EA68FC79AD8";// ���棺�¶ȹ���

//	uchar code TEL[] = {0x22,0x31,0x33,0x30,0x32,0x35,0x31,0x30,0x30,0x30,0x34,0x36,0x22,0x0D};	// "13025100046"�س�
//	uchar code TEL[] = {0x22,0x31,0x33,0x32,0x33,0x32,0x37,0x34,0x30,0x36,0x30,0x38,0x22,0x0D,0x0a};	// "13232740608"�س� ����
	uchar code TEL[] = {0x22,0x31,0x33,0x31,0x32,0x39,0x34,0x33,0x33,0x30,0x33,0x37,0x22,0x0D,0x0a};	// "13129433037"�س� ����
//	uchar code TEL[] = {0x22,0x31,0x35,0x35,0x31,0x32,0x36,0x38,0x34,0x39,0x33,0x37,0x22,0x0D,0x0a};	// "15512684937"�س� ����

	//uchar code TAB3[] = "62A58B666E295EA68BBE7F6E5B8C6BD5";			// �����¶��������
	//uchar code TAB4[] = "7EE7753556685DF262535F00";					// �̵����Ѵ�
	//
	//uchar code TAB5[] = "62535F007EE775355668";				// �򿪼̵���
	//uchar code TAB6[] = "517395ED7EE775355668";				// �رռ̵���
	//uchar code TAB7[] = "83B753D65F53524D6E295EA6";			// ��ȡ��ǰ�¶�
	//uchar code TAB8[] = "8BBE7F6E62A58B666E295EA6FF1A";		// ���ñ����¶ȣ�
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
	GSM_TX(GSM_CMGF);	// �����ı�ģʽ
	GSM_RX(GSM_CMGF);
	DelayMs(100);
	GSM_TX(GSM_CMGS);	// ���õ绰����
	for(i = 0; i < 15; i++)		// ���͵绰����
	{
		SBUF = TEL[i];
		while(!TI);
		TI = 0;
	}
	GSM_RX(GSM_CMGS);
	DelayMs(100);			
	for(i = 0; i < send_len; i++)		// ����PDU��
	{
		SBUF = *(psend_buffer+i);
		while(!TI);
		TI = 0;
	}
	DelayMs(100);
	SBUF = 0x1a;						// ����0x1A
	while(!TI);
	TI = 0;
	// ��ȡ�Ƿ�ok
}

void GSM_RX(uchar value)	// ����ͺ�Ľ���
{
	uchar tmp;
	uint amount = 500;
	switch(value)
	{
		case GSM_CMGF:		// �����ı�ģʽ
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// ���ڽ��յ�������װ��TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// ���յ�K���س�������
						{
							SIZE = COUNT;	// �����ַ���������
							COUNT = 0;
							GSM_cmgf_ok = 1;
							break;									
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// ���յ�R���س�������
						{
							COUNT = 0;
							GSM_cmgf_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// ����
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
		case GSM_CSMP:	// ����sms��ʽ
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// ���ڽ��յ�������װ��TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// ���յ�K���س�������
						{
							SIZE = COUNT;	// �����ַ���������
							COUNT = 0;
							GSM_csmp_ok = 1;
							break;		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// ���յ�R���س�������
						{
							COUNT = 0;
							GSM_csmp_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// ����
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
		case GSM_CMGS:	// ���뷢�ͺ�
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// ���ڽ��յ�������װ��TEMP
						if(0x20 == TEMP[COUNT] && 0x3e == TEMP[COUNT-1])		// ���յ�"> "
						{
							SIZE = COUNT;	// �����ַ���������
							COUNT = 0;
							GSM_cmgs_ok = 1;  // ���뷢�����
							break;		// ����									
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// ���յ�R���س�������
						{
							COUNT = 0;
							GSM_cmgs_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// ����
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
		case GSM_TSIM:	// SIM�����
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// ���ڽ��յ�������װ��TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// ���յ�K���س�������
						{
							SIZE = COUNT;	// �����ַ���������
							COUNT = 0;
							break;			// ������ɣ�����		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// ���յ�R���س�������
						{
							COUNT = 0;
							GSM_tsim_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// ����
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
		case GSM_COPS:	// ��Ӫ�̼��
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// ���ڽ��յ�������װ��TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// ���յ�K���س�������
						{
							SIZE = COUNT;	// �����ַ���������
							COUNT = 0;
							break;			// ������ɣ�����		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// ���յ�R���س�������
						{
							COUNT = 0;
							GSM_cops_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// ����
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
		case GSM_CSQ:	// �źż��
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// ���ڽ��յ�������װ��TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// ���յ�K���س�������
						{
							SIZE = COUNT;	// �����ַ���������
							COUNT = 0;
							break;			// ������ɣ�����		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// ���յ�R���س�������
						{
							COUNT = 0;
							GSM_csq_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// ����
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
		case GSM_CNMI:	// ���ö��Ž�������
		{
			while(1)
			{
				if(RI)
				{
					tmp = SBUF;
					if('A' != tmp && 'T' != tmp)
					{
						TEMP[COUNT] = tmp;		// ���ڽ��յ�������װ��TEMP
						if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x4B == TEMP[COUNT-2])		// ���յ�K���س�������
						{
							SIZE = COUNT;	// �����ַ���������
							COUNT = 0;
							GSM_cnmi_ok = 1;	// ���Ž��������������
							break;			// ��ɣ�����		
						}
						else if(0x0A == TEMP[COUNT] && 0x0D == TEMP[COUNT-1] && 0x52 == TEMP[COUNT-2])	// ���յ�R���س�������
						{
							COUNT = 0;
							GSM_cnmi_ok = 0;
							break;
						}
						else
							COUNT++;
						if(COUNT > 200)		// ����
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


void GSM_TX(uchar value)	// ���ڷ���GSM ATָ��
{
	uchar len,i;
	uchar *pfunction;
	//				  ����
	uchar code TSIM[] = {8,0x41,0x54,0x25,0x54,0x53,0x49,0x4D,0x0D}; 						// AT%TSIM�س�
	uchar code CSQ[] = {7,0x41,0x54,0x2B,0x43,0x53,0x51,0x0D}; 								// AT+CSQ�س�
	uchar code CMGF[] = {10,0x41,0x54,0x2B,0x43,0x4D,0x47,0x46,0x3D,0x31,0x0D}; 			// AT+CMGF=x�س�
	uchar code CMGS[] = {8,0x41,0x54,0x2B,0x43,0x4D,0x47,0x53,0x3D}; 					// AT+CMGS=
	uchar code CNMI[] = {12,0x41,0x54,0x2B,0x43,0x4E,0x4D,0x49,0x3D,0x32,0x2C,0x31,0x0D}; 	// AT+CNMI=2,1�س�
	uchar code CMGR[] = {10,0x41,0x54,0x2B,0x43,0x4D,0x47,0x52,0x3D,0x78,0x0D}; 			// AT+CMGR=x�س�
	uchar code COPS[] = {9,0x41,0x54,0x2B,0x43,0x4F,0x50,0x53,0x3F,0x0D}; 			// AT+COPS?�س�
	uchar code CSMP[] = {19,0x41,0x54,0x2B,0x43,0x53,0x4D,0x50,0x3D,0x31,0x37,0x2C,0x31,0x36,0x37,0x2C,0x30,0x2C,0x38,0x0D};// ����ǰ����	
	//AT+CSMP=17,167,0,8
	switch(value)
	{
		case GSM_TSIM:	// ��⿨
		{
			len = TSIM[0];
			pfunction = TSIM;
		}		
		break;
		case GSM_CSQ:	// ����ź�
		{
			len = CSQ[0];
			pfunction = CSQ;		
		}
		break;
		case GSM_CMGF:	// �����ı�ģʽ
		{
			len = CMGF[0];
			pfunction = CMGF;		
		}
		break;
		case GSM_CMGS:	// ����绰���� ǰ
		{
			len = CMGS[0];
			pfunction = CMGS;		
		}
		break;
		case GSM_CNMI:	// ���ö�������
		{
			len = CNMI[0];
			pfunction = CNMI;		
		}
		break;
		case GSM_CMGR:	// ���Ŷ�ȡ
		{
			len = CMGR[0];		
			pfunction = CMGR;
		}		
		break;
		case GSM_CSMP:	// ���ö��Ÿ�ʽ
		{
			len = CSMP[0];		
			pfunction = CSMP;
		}
		break;
		case GSM_COPS:	// ��ѯ��Ӫ��
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
		GSM_tsim_ok = 1;	// SIM����λ
	}
	else if('0' == TEMP[SIZE-8])
	{
		GSM_tsim_ok = 0;	// SIM������λ
	}					
}

void GSM_CSQ_manage(void)
{
	CSQ[1] = TEMP[SIZE-12];		// �źŸ�λ
	CSQ[0] = TEMP[SIZE-13];		// �ź�ʮλ
//	if(CSQ[1] != '9')
		GSM_csq_ok = 1;
}

void GSM_COPS_manage(void)
{
	if(0x22 == TEMP[SIZE-8] && 0x4D == TEMP[SIZE-9])	// (M") ��ͨ
	{
		GSM_CUCC = 1;
		GSM_cops_ok = 1;	// ��Ӫ�̼�����	
	}
	else if('0' == TEMP[SIZE-8])	// δ��ȡ��Ӫ��
	{
		GSM_CUCC = 0;	
		GSM_CMCC = 0;
	}
//	else if(0x22 == TEMP[SIZE-8] && 0x4D == TEMP[SIZE-9])	// (*") �ƶ�
//	{
//	GSM_CMCC = 1;			
//	GSM_cops_ok = 1;	// ��Ӫ�̼�����	
//	}	
}

void GSM_Init(void)
{
	uchar i;	
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_TSIM);		// ���ͼ��SIM��
		GSM_RX(GSM_TSIM);		// ����GSMģ�鴫������
		DelayMs(100);
		GSM_TSIM_manage();		// �жϽ�������
		GSM_RX_clear();			// ��ս�������
		if(GSM_tsim_ok)
			break;		// ��⵽SIM����ǰ����				
	}
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_COPS);		// ���ͼ����Ӫ��
		GSM_RX(GSM_COPS);		// ����GSMģ�鴫������
		DelayMs(100);
		GSM_COPS_manage();		// �жϽ�������
		GSM_RX_clear();			// ��ս�������
		if(GSM_cops_ok)
			break;		// ��⵽��Ӫ����ǰ����						
	}
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_CSQ);		// ���ͼ���ź�
		GSM_RX(GSM_CSQ);		// ����GSMģ�鴫������
		DelayMs(100);
		GSM_CSQ_manage();		// �жϽ�������
		GSM_RX_clear();			// ��ս�������
		if(GSM_csq_ok)
			break;		// ��⵽�ź���ǰ����	
	}	
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_CNMI);		// �������ö�������
		GSM_RX(GSM_CNMI);		// ����GSMģ�鴫������
		DelayMs(100);
		GSM_RX_clear();			// ��ս�������
		if(GSM_cnmi_ok)			
			break;		// ������� ��ǰ����				
	}	
	for(i = 0; i < 10; i++)
	{
		GSM_TX(GSM_CSMP);		// ��������SMS��ʽ
		GSM_RX(GSM_CSMP);		// ����GSMģ�鴫������
		DelayMs(100);
		GSM_RX_clear();			// ��ս�������
		if(GSM_csmp_ok)
			break;	// ������� ��ǰ����				
	}
}

/*  11.0592Mhz	Uart���ڳ�ʼ��		    */
void Uart_Init(void) 	// ���ڳ�ʼ��
{
	TMOD &= 0x0F;	// ������ʱ��0���ã���ն�ʱ��1����
	TMOD |= 0x20;	// ��ʱ��1 ģʽ2
	SCON |= 0x50;	// ���ڷ�ʽ1��8λUART�����ʿɱ䣬�ɽ���
	TH1   = 0xFD;	// TL1�����TH1��ֵװ��
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// �رն�ʱ��1�ж�
//	ES    = 1;		// �򿪴����ж�
//	EA    = 1;	    // �����ж�
	TR1   = 1;		// ����ʱ��1
}
