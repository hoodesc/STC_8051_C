#include <stc15.h>
#include <intrins.h>

typedef unsigned char uint8;
typedef unsigned int uint16;

#define CMD_IDLE 		0		//	����ģʽ
#define CMD_READ 		1		//	IAP�ֽڶ�����
#define CMD_PROGRAM 2		//	IAP�ֽڱ������
#define CMD_ERASE 	3		//	IAP������������

#define ENABLE_IAP 	0x80	//	SYSCLK<30MHz
#define IAP_ADDRESS	0x3300	//	��д������ַ


void Iapidle()
{
	IAP_CONTR = 0;		//	�ر�IAP����
	IAP_CMD 	= 0;			//	�������Ĵ���
	IAP_TRIG	= 0;			//	��������Ĵ���
	IAP_ADDRH = 0x80;	//	����ַ���õ���IAP����
	IAP_ADDRL = 0x0;		
}

/*****************
��ISP/IAP/EEPROM�����һ�ֽ�
******************/
uint8 IapReadByte(uint16 addr)
{
	uint8 dat;		//	���ݻ�����
	IAP_CONTR = ENABLE_IAP;			//	ʹ��IAP
	IAP_CMD = CMD_READ;					//	����IAP����
	IAP_ADDRL = addr;						//	����IAP�͵�ַ
	IAP_ADDRH = addr >> 8;			//	����IAP�ߵ�ַ
	IAP_TRIG = 0x5a;						//	д��������(0x5a)
	IAP_TRIG = 0xa5;						//	д��������(0xa5)
	_nop_();										//	�ȴ��������
	dat = IAP_DATA;							//	��ȡEEPROM����
	Iapidle();									//	�ر�IAP����
	return dat;									//	��������
}

/******************
дһ�ֽ����ݵ�EEPRPOM
******************/
void IapProgramByte(uint16 addr,uint8 dat)
{
	IAP_CONTR = ENABLE_IAP;				//	ʹ��IAP
	IAP_CMD = CMD_PROGRAM;				//	����IAP����
	IAP_ADDRL = addr;							//	����IAP�͵�ַ
	IAP_ADDRH = addr >> 8;					//	����IAP�ߵ�ַ
	IAP_DATA = dat;								//	д���ݵ�EEPROM
	IAP_TRIG = 0x5a;							//	д��������(0x5a)
	IAP_TRIG = 0xa5;							//	д��������(0xa5)
	_nop_();											//	�ȴ��������
	Iapidle();										//	�ر�IAP����
}

/********************
��������
********************/
void IapEraseSector(uint16 addr)
{
	IAP_CONTR = ENABLE_IAP;				//	ʹ��IAP
	IAP_CMD = CMD_ERASE;					//	����IAP����
	IAP_ADDRL = addr;							//	����IAP�͵�ַ
	IAP_ADDRH = addr >> 8;				//	����IAP�ߵ�ַ
	IAP_TRIG = 0x5a;							//	д��������(0x5a)
	IAP_TRIG = 0xa5;							//	д��������(0xa5)
	_nop_();											//	�ȴ��������
	Iapidle();										//	�ر�IAP����	
}

void Delay100ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 10;
	j = 31;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


/*  11.0592Mhz	Uart���ڳ�ʼ��		    */
void Uart_Init()	// ���ڳ�ʼ��
{
	SCON = 0x50;		//8???,?????
	AUXR |= 0x40;		//???1???Fosc,?1T
	AUXR &= 0xFE;		//??1?????1???????
	TMOD &= 0x0F;		//?????1?16???????
	TL1 = 0x8F;		//??????
	TH1 = 0xFD;		//??????
	ET1 = 0;		//?????1??
	TR1 = 1;		//?????1
}
/**********************************/


void Send_One_byte(uint8 dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
}

void main()
{
	uint16 i;
	Uart_Init();
	Delay100ms();
	while(1)
	{
		Send_One_byte(0x01);
		IapEraseSector(IAP_ADDRESS);		//	��������	
		Send_One_byte(0x02);
		for(i = 0; i < 512; i++)		//	����Ƿ�����ɹ�
		{
			if(IapReadByte(IAP_ADDRESS+i) != 0xff)
				goto Err;					
		}
		Send_One_byte(0x03);
		//	д������
		for(i = 0; i < 512; i++)
		{
			IapProgramByte(IAP_ADDRESS+i,(uint8)i);
		}
		Send_One_byte(0x04);
		//	��������
		for(i = 0; i < 512; i++)
		{
			Send_One_byte(IapReadByte(IAP_ADDRESS+i));
		}
		Send_One_byte('z');
		while(1);
	}
		Err:	
			Send_One_byte(0xFF);
			while(1);	
}