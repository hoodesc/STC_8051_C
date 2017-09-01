#include<STC12C5A.h>
#include"MYTYPE.h"
#include"MQ-2.h"
#include"Delay.h"


bit MQ2_flag = 0;

//void Int0_ISR()	interrupt 0		
//{
//	EX0 = 0;		// 关闭外部中断0
//	MQ2_flag = 1;	// 发现有害气体标识
//					// 主程序循环检测“MQ2”直到稳定低电平	
//}
//
//void Int0_Init()	// 外部中断0初始化
//{
//	IT0 = 1;	// 外部中断0 下降沿触发
//	EX0	= 1;	// 外部中断0 中断允许
//	EA  = 1;	// 开总中断
//}