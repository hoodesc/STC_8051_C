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
		udata = udata >> 1;		// udata右移一位准备下一次送上总线
		if(dat)
		{
			DQ = 0;		// 拉低总线
			delay(1);	// 延时2us
			DQ = 1;		// 送1
			delay(40);	// 延时80us
			DQ = 1;		// 释放总线
			delay(1);	// 延时2us
		}
		else
		{
			DQ = 0;		// 拉低总线
			delay(1);	// 延时2us
			DQ = 0;		// 送0
			delay(40);	// 延时80us;
			DQ = 1;		// 释放总线
			delay(1);	// 延时2us;
		}
	}	
}

uchar DS18B20_Read()
{
	uchar i;
	uchar udata = 0x00;
	for(i = 0; i < 8; i++)
	{
		udata = udata >> 1;	// udata右移一位，第一次移位为空操作
		DQ = 0;			// 拉低总线
		delay(2);		// 延时4us，产生读时隙
		DQ = 1;			// 拉高总线
		if(DQ)
			udata |= 0x80;		// 1：读DQ数据并存放在udata最高位
		else
			udata |= 0x00;		// 0：读DQ数据并存放在udata最高位
		delay(40);		// 延时80us
		DQ = 1;			// 释放总线
		delay(1);		// 延时2us
	}
	return(udata);	
}

void DS18B20_Init()
{
	DQ = 1;			// 拉高总线
	delay(1);
	
	DQ = 0;			// 拉低总线
	delay(350);		// 延时700us	
	DQ = 1;			// 释放总线
	delay(15);		// 延时30us
	while(DQ);		// 等待DS18B20拉低总线
		//LED = 0;	// 复位成功，点亮LED
	delay(100);		// 延时200us
	DQ = 1;			// 释放总线
	delay(150);		// 延时300us
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
	// 初始化并启动温度转换
	DS18B20_Init();			// DS18B20初始化
	DS18B20_Write(0xCC);	// 跳过ROM地址
	DS18B20_Write(0x44);	// 启动温度转换
	DelayMs(1000);
	// 初始化，读温度
	DS18B20_Init();			// DS18B20初始化
	DS18B20_Write(0xCC);	// 跳过ROM地址
	DS18B20_Write(0xBE);	// 读取RAM命令
//		Low = DS18B20_Read();
//		Hight = DS18B20_Read();
	compute(DS18B20_Read(),DS18B20_Read());		// 	先读低8位，再读高八位
//	for(i = 0; i < 5; i++)	// 串口输出
//	{
//		SBUF = c[i];
//		while(!TI);
//		TI = 0;
//	}
}