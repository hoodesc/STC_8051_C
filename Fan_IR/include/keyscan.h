#ifndef __keyscan_h__
#define __keyscan_h__

sbit IR_DAT = P3^3;
extern void Timer0_Init(void);
extern void Key1_Scan(void);
extern void Key2_Scan(void);
extern void Key3_Scan(void);

#endif