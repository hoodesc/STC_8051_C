#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
//MCU:STC12C2052AD
//晶振:11.0592Mhz
//检测开机次数
typedef unsigned char uchar;
typedef unsigned int uint;

sbit SDA = P1^7;
sbit SCL = P1^6;

uchar udata[20] = {0x00};
/*      	Uart串口初始化		    */
void Uart_Init()	// 串口初始化
{
	TMOD &= 0x0F;	// 保留后定时器0设置，清空定时器1设置
	TMOD |= 0x20;	// 定时器1 模式2
	SCON |= 0x50;	// 串口方式1，8位UART波特率可变，可接收
	TH1   = 0xFD;	// TL1溢出后将TH1的值装入
	TL1   = 0xFD;	// 9600bps
	ET1   = 0;		// 关闭定时器1中断
	TR1   = 1;		// 开定时器1
}
/**********************************/
/*      	约5us延时函数		    */
void Delay5us(void)		// 4个机器周期延时，大约5us
{
	uchar i;
	for(i = 0; i < 4; i++)
		_nop_();
}
/**********************************/
/*      	毫秒延时函数		    */
void DelayMs(uchar ms)	
{
	int i,j;
	for(i = 0; i < ms; i++)
		for(j = 0; j < 936; j++);
}
/**********************************/
/*      	读到的字节保存到数组	    */
void uData(uchar dat)
{
	static uchar i = 0;
	udata[i] = dat;
	i++;
	if(20 == i)
		i = 0;
}
/**********************************/
/*			IIC起始信号		        */
void IIC_Start()
{
	SDA = 1;
	Delay5us();
	SCL = 1;
	Delay5us();
	SDA = 0;
	Delay5us();
}
/**********************************/
/*			IIC停止信号		        */
void IIC_Stop()
{
	SDA = 0;
	Delay5us();
	SCL = 1;
	Delay5us();
	SDA = 1;
	Delay5us();
}
/**********************************/
/*			ACK应答信号		        */
void IIC_Ack()
{
	SCL = 0;
	SDA = 0;
	SCL = 1;
	Delay5us();
	SCL = 0;
	SDA = 1;		// 释放总线
}
/**********************************/
/*			ACK否应答信号		      */
void IIC_noAck()
{
	SCL = 0;
	SDA = 1;
	SCL = 1;
	Delay5us();
	SCL = 0;
	SDA = 1;		// 释放总线
}
/**********************************/
/*			读ACK应答信号		       */
bit IIC_Rd_Ack()
{
	bit ACK;
	uchar i = 0;
	SCL = 1;
	Delay5us();
	while((1 == SDA) && (i < 255))		
		i++;	
	ACK = SDA;
	SCL = 0;
	Delay5us();
	return ACK;
}
/**********************************/
/*			IIC一字节写入		      */
void IIC_Send_Byte(uchar dat)       
{
	uchar i;
	for(i = 0; i < 8; i++)
	{
		SCL = 0;
		Delay5us();
		SDA = (dat & 0x80) >> 7;
		Delay5us();
		SCL = 1;
		Delay5us();
		dat <<= 1;
	}
	SCL = 0;
	SDA = 1;		// 释放总线
	Delay5us();
}
/**********************************/
/*			IIC一字节读出		      */
uchar IIC_receive_Byte()
{
	uchar i;
	uchar dat = 0;
	for(i = 0; i < 8; i++)
	{
		SCL = 0;
		Delay5us();
		SCL = 1;
		Delay5us();
		dat <<= 1;
		if(SDA)
			dat |= 0x01;	
	}
	uData(dat);
	SCL = 0;
	return dat;
}
/**********************************/
/*			AT24C02连续写入		      */
void IIC_WrData(uchar DeviceAdd,uchar FirstAdds,uchar dat,uchar nLen)	
{	//				  器件地址		  子地址		  8位数据   写次数
	//开始；器件地址+写、读应答；子地址、读应答；数据、读应答；结束
	uchar i;
	IIC_Start();	    		// 开始
	IIC_Send_Byte(DeviceAdd);	// 写器件地址 默认：1010 A2A1A0+w/r	 写
	IIC_Rd_Ack();				// 读应答
	IIC_Send_Byte(FirstAdds);	// 写器件子地址
	IIC_Rd_Ack();				// 读应答
	for(i = 0; i < nLen; i++)	// 连续写nLen次
	{
	 	IIC_Send_Byte(dat);		// 写一字节数据
		IIC_Rd_Ack();			// 读应答
	}
	IIC_Stop();		    		// 结束
}
/**********************************/
/*			AT24C02连续读出		      */
void IIC_RdData(uchar DeviceAdd,uchar FirstAdds,uchar nLen)				
{	//				  器件地址        子地址		  读次数
	//开始 - 器件地址+写、读应答 - 子地址、读应答 - 开始 - 器件地址、读应答 - 读数据、写否应答 - 结束
	uchar i;
	IIC_Start();	    					// 开始
	IIC_Send_Byte(DeviceAdd);				// 写待读器件地址 默认：1010 A2A1A0+w/r	  写
	IIC_Rd_Ack();							// 读应答
	IIC_Send_Byte(FirstAdds);				// 写待读器件子地址	
	IIC_Rd_Ack();							// 读应答
	IIC_Start();	    					// 开始
	IIC_Send_Byte(DeviceAdd |= 0x01);		// 写器件地址最后一位改为1，即“读”
	IIC_Rd_Ack();							// 读应答
	for(i = 0; i < nLen; i++)
	{
		IIC_receive_Byte();					// 读一字节数据
		if(nLen-1 == i)						// 如果最后一个地址数据读完	写否应答
			IIC_noAck();
		else								// 否则 写应答
			IIC_Ack();
	}	
	IIC_Stop();		    					// 结束	
}
/**********************************/
/*			主函数		      */
void main()
{	
	Uart_Init();
	IIC_RdData(0xA0,0xFF,1);			//器件地址	子地址	读次数
	udata[0] = udata[0] + 0x01;
	DelayMs(5);
	IIC_WrData(0xA0,0xFF,udata[0],1);	//器件地址	子地址	8位数据   写次数

	TI = 1;
	printf("累计开机:%d次\n",(uint)udata[0]);
	while(!TI);
	TI = 0;
	while(1);
}
/**********************************/