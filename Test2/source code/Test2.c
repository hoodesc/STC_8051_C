#include <stc12c2052ad.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned int uint;

//	11.0592��ʱ��������λms
void DelayMs(uchar time)
{
	uchar i,j;
	while(time--)
	{
		for(i = 0; i < 9; i++)
			for(j = 0; j < 177; j++);
	}	
}

void main(void)
{
	CMOD = 0x02; // ���� PCA ��ʱ��
	CL = 0x00;//����
	CH = 0x00;
	CCAP0L = 0x40; //����75%ռ�ձ�
	CCAP0H = 0x40; // ��CL��ff��Ϊ00���ʱ��CCAP0Hװ�ص�CCAP0L�У�ʵ���޸��ŵĸ���PWM��
	CCAPM0 = 0x42; //0100,0010 ����ʹ��PWMģʽ
	CR = 1; //���� PCA ��ʱ��.
	while(1){};
}