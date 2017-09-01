#include <stc15.h>
#include <intrins.h>

typedef unsigned char uint8;
typedef unsigned int uint16;

#define CMD_IDLE 		0		//	空闲模式
#define CMD_READ 		1		//	IAP字节读命令
#define CMD_PROGRAM 2		//	IAP字节编程命令
#define CMD_ERASE 	3		//	IAP扇区擦除命令

#define ENABLE_IAP 	0x80	//	SYSCLK<30MHz
#define IAP_ADDRESS	0x3300	//	读写扇区地址


void Iapidle()
{
	IAP_CONTR = 0;		//	关闭IAP功能
	IAP_CMD 	= 0;			//	清除命令寄存器
	IAP_TRIG	= 0;			//	清除触发寄存器
	IAP_ADDRH = 0x80;	//	将地址设置到非IAP区域
	IAP_ADDRL = 0x0;		
}

/*****************
从ISP/IAP/EEPROM区域读一字节
******************/
uint8 IapReadByte(uint16 addr)
{
	uint8 dat;		//	数据缓冲区
	IAP_CONTR = ENABLE_IAP;			//	使能IAP
	IAP_CMD = CMD_READ;					//	设置IAP命令
	IAP_ADDRL = addr;						//	设置IAP低地址
	IAP_ADDRH = addr >> 8;			//	设置IAP高地址
	IAP_TRIG = 0x5a;						//	写触发命令(0x5a)
	IAP_TRIG = 0xa5;						//	写触发命令(0xa5)
	_nop_();										//	等待操作完成
	dat = IAP_DATA;							//	读取EEPROM数据
	Iapidle();									//	关闭IAP功能
	return dat;									//	返回数据
}

/******************
写一字节数据到EEPRPOM
******************/
void IapProgramByte(uint16 addr,uint8 dat)
{
	IAP_CONTR = ENABLE_IAP;				//	使能IAP
	IAP_CMD = CMD_PROGRAM;				//	设置IAP命令
	IAP_ADDRL = addr;							//	设置IAP低地址
	IAP_ADDRH = addr >> 8;					//	设置IAP高地址
	IAP_DATA = dat;								//	写数据到EEPROM
	IAP_TRIG = 0x5a;							//	写触发命令(0x5a)
	IAP_TRIG = 0xa5;							//	写触发命令(0xa5)
	_nop_();											//	等待操作完成
	Iapidle();										//	关闭IAP功能
}

/********************
擦除扇区
********************/
void IapEraseSector(uint16 addr)
{
	IAP_CONTR = ENABLE_IAP;				//	使能IAP
	IAP_CMD = CMD_ERASE;					//	设置IAP命令
	IAP_ADDRL = addr;							//	设置IAP低地址
	IAP_ADDRH = addr >> 8;				//	设置IAP高地址
	IAP_TRIG = 0x5a;							//	写触发命令(0x5a)
	IAP_TRIG = 0xa5;							//	写触发命令(0xa5)
	_nop_();											//	等待操作完成
	Iapidle();										//	关闭IAP功能	
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


/*  11.0592Mhz	Uart串口初始化		    */
void Uart_Init()	// 串口初始化
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
		IapEraseSector(IAP_ADDRESS);		//	擦除扇区	
		Send_One_byte(0x02);
		for(i = 0; i < 512; i++)		//	检测是否擦除成功
		{
			if(IapReadByte(IAP_ADDRESS+i) != 0xff)
				goto Err;					
		}
		Send_One_byte(0x03);
		//	写入数据
		for(i = 0; i < 512; i++)
		{
			IapProgramByte(IAP_ADDRESS+i,(uint8)i);
		}
		Send_One_byte(0x04);
		//	读出数据
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