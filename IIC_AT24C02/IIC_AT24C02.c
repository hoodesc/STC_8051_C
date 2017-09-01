#include <reg52.h>
#include <stdio.h>
#include <intrins.h>
//MCU:STC12C2052AD
//����:11.0592Mhz
//��⿪������
typedef unsigned char uchar;
typedef unsigned int uint;

sbit SDA = P1^7;
sbit SCL = P1^6;

uchar udata[20] = {0x00};
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
/*      	Լ5us��ʱ����		    */
void Delay5us(void)		// 4������������ʱ����Լ5us
{
	uchar i;
	for(i = 0; i < 4; i++)
		_nop_();
}
/**********************************/
/*      	������ʱ����		    */
void DelayMs(uchar ms)	
{
	int i,j;
	for(i = 0; i < ms; i++)
		for(j = 0; j < 936; j++);
}
/**********************************/
/*      	�������ֽڱ��浽����	    */
void uData(uchar dat)
{
	static uchar i = 0;
	udata[i] = dat;
	i++;
	if(20 == i)
		i = 0;
}
/**********************************/
/*			IIC��ʼ�ź�		        */
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
/*			IICֹͣ�ź�		        */
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
/*			ACKӦ���ź�		        */
void IIC_Ack()
{
	SCL = 0;
	SDA = 0;
	SCL = 1;
	Delay5us();
	SCL = 0;
	SDA = 1;		// �ͷ�����
}
/**********************************/
/*			ACK��Ӧ���ź�		      */
void IIC_noAck()
{
	SCL = 0;
	SDA = 1;
	SCL = 1;
	Delay5us();
	SCL = 0;
	SDA = 1;		// �ͷ�����
}
/**********************************/
/*			��ACKӦ���ź�		       */
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
/*			IICһ�ֽ�д��		      */
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
	SDA = 1;		// �ͷ�����
	Delay5us();
}
/**********************************/
/*			IICһ�ֽڶ���		      */
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
/*			AT24C02����д��		      */
void IIC_WrData(uchar DeviceAdd,uchar FirstAdds,uchar dat,uchar nLen)	
{	//				  ������ַ		  �ӵ�ַ		  8λ����   д����
	//��ʼ��������ַ+д����Ӧ���ӵ�ַ����Ӧ�����ݡ���Ӧ�𣻽���
	uchar i;
	IIC_Start();	    		// ��ʼ
	IIC_Send_Byte(DeviceAdd);	// д������ַ Ĭ�ϣ�1010 A2A1A0+w/r	 д
	IIC_Rd_Ack();				// ��Ӧ��
	IIC_Send_Byte(FirstAdds);	// д�����ӵ�ַ
	IIC_Rd_Ack();				// ��Ӧ��
	for(i = 0; i < nLen; i++)	// ����дnLen��
	{
	 	IIC_Send_Byte(dat);		// дһ�ֽ�����
		IIC_Rd_Ack();			// ��Ӧ��
	}
	IIC_Stop();		    		// ����
}
/**********************************/
/*			AT24C02��������		      */
void IIC_RdData(uchar DeviceAdd,uchar FirstAdds,uchar nLen)				
{	//				  ������ַ        �ӵ�ַ		  ������
	//��ʼ - ������ַ+д����Ӧ�� - �ӵ�ַ����Ӧ�� - ��ʼ - ������ַ����Ӧ�� - �����ݡ�д��Ӧ�� - ����
	uchar i;
	IIC_Start();	    					// ��ʼ
	IIC_Send_Byte(DeviceAdd);				// д����������ַ Ĭ�ϣ�1010 A2A1A0+w/r	  д
	IIC_Rd_Ack();							// ��Ӧ��
	IIC_Send_Byte(FirstAdds);				// д���������ӵ�ַ	
	IIC_Rd_Ack();							// ��Ӧ��
	IIC_Start();	    					// ��ʼ
	IIC_Send_Byte(DeviceAdd |= 0x01);		// д������ַ���һλ��Ϊ1����������
	IIC_Rd_Ack();							// ��Ӧ��
	for(i = 0; i < nLen; i++)
	{
		IIC_receive_Byte();					// ��һ�ֽ�����
		if(nLen-1 == i)						// ������һ����ַ���ݶ���	д��Ӧ��
			IIC_noAck();
		else								// ���� дӦ��
			IIC_Ack();
	}	
	IIC_Stop();		    					// ����	
}
/**********************************/
/*			������		      */
void main()
{	
	Uart_Init();
	IIC_RdData(0xA0,0xFF,1);			//������ַ	�ӵ�ַ	������
	udata[0] = udata[0] + 0x01;
	DelayMs(5);
	IIC_WrData(0xA0,0xFF,udata[0],1);	//������ַ	�ӵ�ַ	8λ����   д����

	TI = 1;
	printf("�ۼƿ���:%d��\n",(uint)udata[0]);
	while(!TI);
	TI = 0;
	while(1);
}
/**********************************/