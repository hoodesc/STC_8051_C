#include<STC12C5A.h>
#include"MYTYPE.h"
#include"DS18B20.h"
#include"delay.h"

uchar code number[] = {'0','1','2','3','4','5','6','7','8','9'};
uchar c[] = {'0','0','.','0',0xa1,0xe6,'\0'};

sbit DQ = P3^6;
bit DS18B20_flag = 0;

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
	while(DQ);		// �ȴ�DS18B20��������
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
	c[0] = number[bai];
	c[1] = number[shi];
	c[3] = number[ge];
}
void DS18B20_Temperature_Read()
{
//	uchar i;
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
//	for(i = 0; i < 5; i++)	// �������
//	{
//		SBUF = c[i];
//		while(!TI);
//		TI = 0;
//	}
}