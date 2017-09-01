#ifndef __A6_H__
#define __A6_H__


#define Buf1_Max 60 					  //串口1缓存长度
extern uchar Register_Flag;  //A6  注册标志位  1 ：注册成功  3：注册失败   5：注册成功，但处于漫游状态下，无法拨打电话发短信
extern uchar First_Int;
extern uchar Uart1_Buf[];

extern void CLR_Buf1(void);
extern uchar Find(uchar *a);
extern void Second_AT_Command(uchar *b,uchar *a,uchar wait_time);  
extern void Set_Text_Mode(void);
extern void Send_Message(uchar *pdat);
extern void Wait_CREG(void);

//发送短信内容
extern code uchar mq2_normal[];
extern code uchar mq2_warning[];
extern code uchar huoyan_warning[];
extern code uchar huoyan_normal[];

#endif