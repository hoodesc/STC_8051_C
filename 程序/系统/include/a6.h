#ifndef __A6_H__
#define __A6_H__


#define Buf1_Max 60 					  //����1���泤��
extern uchar Register_Flag;  //A6  ע���־λ  1 ��ע��ɹ�  3��ע��ʧ��   5��ע��ɹ�������������״̬�£��޷�����绰������
extern uchar First_Int;
extern uchar Uart1_Buf[];

extern void CLR_Buf1(void);
extern uchar Find(uchar *a);
extern void Second_AT_Command(uchar *b,uchar *a,uchar wait_time);  
extern void Set_Text_Mode(void);
extern void Send_Message(uchar *pdat);
extern void Wait_CREG(void);

//���Ͷ�������
extern code uchar mq2_normal[];
extern code uchar mq2_warning[];
extern code uchar huoyan_warning[];
extern code uchar huoyan_normal[];

#endif