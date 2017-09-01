#include<STC12C2052AD.H> //头文件
sbit LED1 = P3 ^ 0;
sbit LED2 = P3 ^ 1;
sbit LED3 = P3 ^ 2;
sbit LED4 = P3 ^ 3;
sbit LED5 = P3 ^ 4;
sbit LED6 = P3 ^ 5;
//左边6个
sbit LED7 = P1 ^ 7;
sbit LED8 = P1 ^ 6;
sbit LED9 = P1 ^ 5;
sbit LED10 = P1 ^ 4;
sbit LED11 = P1 ^ 3;
sbit LED12 = P1 ^ 2;
sbit LED13 = P1 ^ 1;
sbit LED14 = P1 ^ 0;
//右边8个
sbit KEY = P3 ^ 7; //待用

/*********************************************************************************************
函数名：毫秒级CPU延时函数
调  用：DELAY_MS (?);
参  数：1~65535（参数不可为0）
返回值：无
结  果：占用CPU方式延时与参数数值相同的毫秒时间
备  注：应用于1T单片机时i<600，应用于12T单片机时i<125
/*********************************************************************************************/
void DELAY_MS (unsigned int a){
	unsigned int i;
	while( a-- != 0){
		for(i = 0; i < 600; i++);
	}
}
/*********************************************************************************************/
void main (void){   
	unsigned char b;
	for(b=0;;b++){
	unsigned char a;
		for(a=0;a<1;){
		LED1=a;
		a=1;
		DELAY_MS(100);
		LED1=a;
		DELAY_MS(100);
		}
		a=0;
		for(a=0;a<1;){
		LED2=a;
		DELAY_MS(100);
		a=1;
		LED2=a;
		DELAY_MS(100);
		}
		for(a=0;a<1;){
		LED3=a;
		DELAY_MS(100);
		a=1;
		LED3=a;
		DELAY_MS(100);
		}
		for(a=0;a<1;){
		LED4=a;
		DELAY_MS(100);
		a=1;
		LED4=a;
		DELAY_MS(100);
		}
		for(a=0;a<1;){
		LED1=a;
		LED2=a;
		LED3=a;
		LED4=a;
		DELAY_MS(100);
		a=1;
		LED1=a;
		LED2=a;
		LED3=a;
		LED4=a;
		DELAY_MS(100);
		}
	}
}