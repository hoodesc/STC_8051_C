#include<STC12C2052AD.H> //ͷ�ļ�

//unsigned char a;
sbit LED = P1 ^ 7;
sbit KEY = P1 ^ 0;

void main (void){
	//PWM_init(); //PWM��ʼ��
	KEY = 1;
	LED = 1;
	while(1){
		
	LED = KEY;
/*	if(KEY == 0){
		 LED = 0;
		 }
	if(KEY == 1){
		 LED =1;
		}
*/
}
}
