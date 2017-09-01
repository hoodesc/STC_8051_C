#include <reg52.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned int uint;

uchar code Number[] = {'0','1','2','3','4','5','6','7','8','9'};
//uchar  c[] = {'0','0','.','0','\n'};
uchar Tem[6] = {0,0,'.',0,0xA1,0xE6};

sbit DQ = P2^2;		// ��������
sbit LED = P1^6;
void delay(uint us);
void DelayMs(uint ms);
void compute(uchar lsb,uchar msb);

//	11.0592Mhz Լ1ms��ʱ
void DelayMs(uint ms)
{
	uchar i,j;
	while(ms--)
	{
		for(i = 0; i < 9; i++)
			for(j = 0; j < 177; j++);
	}	
}
// 11.0592Mhz Լ2us��ʱ
void delay(uint us)		
{	
	uint i;
	for(i = 0; i < us; i++)
		_nop_();
}

void DS18B20_Write(uchar udata)
{
	uchar i;
	uchar dat;
	for(i = 0; i < 8; i++)
	{
		dat = udata & 0x01;
		udata = udata >> 1;		// udata����һλ׼����һ����������
		if(dat)
		{
			DQ = 0;		// ��������
			delay(1);	// ��ʱ2us
			DQ = 1;		// ��1
			delay(40);	// ��ʱ80us
			DQ = 1;		// �ͷ�����
			delay(1);	// ��ʱ2us
		}
		else
		{
			DQ = 0;		// ��������
			delay(1);	// ��ʱ2us
			DQ = 0;		// ��0
			delay(40);	// ��ʱ80us;
			DQ = 1;		// �ͷ�����
			delay(1);	// ��ʱ2us;
		}
	}	
}

uchar DS18B20_Read()
{
	uchar i;
	uchar udata = 0x00;
	for(i = 0; i < 8; i++)
	{
		udata = udata >> 1;	// udata����һλ����һ����λΪ�ղ���
		DQ = 0;			// ��������
		delay(2);		// ��ʱ4us��������ʱ϶
		DQ = 1;			// ��������
		if(DQ)
			udata |= 0x80;		// 1����DQ���ݲ������udata���λ
		else
			udata |= 0x00;		// 0����DQ���ݲ������udata���λ
		delay(40);		// ��ʱ80us
		DQ = 1;			// �ͷ�����
		delay(1);		// ��ʱ2us
	}
	return(udata);	
}

void DS18B20_Init()
{
	DQ = 1;			// ��������
	delay(1);
	
	DQ = 0;			// ��������
	delay(350);		// ��ʱ700us	
	DQ = 1;			// �ͷ�����
	delay(15);		// ��ʱ30us
//	while(DQ);		// �ȴ�DS18B20��������
		//LED = 0;	// ��λ�ɹ�������LED
	delay(100);		// ��ʱ200us
	DQ = 1;			// �ͷ�����
	delay(150);		// ��ʱ300us
}

void compute(uchar lsb,uchar msb)
{
	uchar ge,shi,bai;
	uint num;
	uint temp;
	num = msb;
	num  <<= 8;
	num |= lsb;
	temp = num*0.0625*10+0.5;	
	bai = temp/100;  	
	shi = temp%100/10;	
	ge  = temp%100%10;	
	Tem[0] = bai;
	Tem[1] = shi;
	Tem[3] = ge;
	Tem[0] = Number[Tem[0]];
	Tem[1] = Number[Tem[1]];
	Tem[3] = Number[Tem[3]];
}

/*      	Uart���ڳ�ʼ��		    */
void Uart_Init()	// ���ڳ�ʼ��
{
	TMOD &= 0x0F;	// ������ʱ��0���ã���ն�ʱ��1����
	TMOD |= 0x20;	// ��ʱ��1 ģʽ2
	SCON |= 0x50;	// ���ڷ�ʽ1��8λUART�����ʿɱ䣬�ɽ���
	TH1   = 0xFD;	// TL1�����TH1��ֵװ��
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// �رն�ʱ��1�ж�
	TR1   = 1;		// ����ʱ��1
}
/**********************************/

void main()
{
	uchar i;
	Uart_Init();	// ���ڳ�ʼ��
	while(1)
	{ 	
		// ��ʼ���������¶�ת��
		DS18B20_Init();			// DS18B20��ʼ��
		DS18B20_Write(0xCC);	// ����ROM��ַ
		DS18B20_Write(0x44);	// �����¶�ת��
		DelayMs(1000);
		// ��ʼ�������¶�
		DS18B20_Init();			// DS18B20��ʼ��
		DS18B20_Write(0xCC);	// ����ROM��ַ
		DS18B20_Write(0xBE);	// ��ȡRAM����
//		Low = DS18B20_Read();
//		Hight = DS18B20_Read();
		compute(DS18B20_Read(),DS18B20_Read());		// 	�ȶ���8λ���ٶ��߰�λ
		for(i = 0; i < 5; i++)
		{
			SBUF = Tem[i];
			while(!TI);
			TI = 0;
		}
//	while(1);		
	}
}