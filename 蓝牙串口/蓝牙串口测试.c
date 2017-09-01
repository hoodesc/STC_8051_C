#include <reg52.h>
sbit led1 = P1^7;
sbit  led2 = P1^6;    //定义红色LED 引脚

void com_ser() interrupt 4    //串口中断处理函数，收到数据后产生中断，在这里处理
{
	unsigned char temp;        //定义一个变量，用来存放电脑端发来的数据

	if(RI)                    //如果数据已经接收完，即RI=1
   {
		RI=0;                //对RI进行清零
		temp = SBUF;        //把收到的数据赋值给变量（注意接收的写法）
	}  

	if(temp=='1')        //判断收到的数据是不是小写的 1
	{
		if(led1==1)
		{
			SBUF = 'O';
			led1 = 0;
		}
			else
			{
				SBUF = 'N';
				led1 = 1;
			}  
	}
 
	if(temp=='2')        //判断收到的数据是不是小写的 1
	{
		if(led2==1)
		{
		SBUF = 'O';
		led2 = 0;
		}
			else
			{
			SBUF = 'N';
			led2 = 1;
			}		  
	}

	if(temp=='0')
	{
	SBUF = 'N';
	led1 = 1;
	led2 = 1;
	}

while(!TI);            //判断数据是否发送完毕，即TI=1，没发完就在这等待
TI = 0;                    //发送完后对TI进行清零
}



void main()                    //主函数
{
/******************设定定时器*********************/
	TMOD = 0x20;            //设定定时器的工作方式（方式2）
	TH1  = 0xfd;            //设定波特率为 9600 bps （在11.0592MHZ晶振时）
	TL1  = 0xfd;
 
/*******************设定串口**********************/
	SM0  = 0;                //SM0 和 SM1 设定串口的工作方式（方式1）
	SM1  = 1;
	REN  = 1;                //允许串口接收外部传来的数据
 
/******************设定中断**********************/
	ES   = 1;                //允许串口收到数据后产生中断通知我们
	EA   = 1;                //因为总中断开关是控制所有中断的，所以要把它打开
	TR1  = 1;                //启动定时器，串口就开始工作喽！

while(1);
}
