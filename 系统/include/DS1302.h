#ifndef __DS1302_H__
#define __DS1302_H__

extern void Write_DS1302_Byte(uchar dat);
extern void Write_DS1302(uchar add,uchar dat);
extern uchar Read_DS1302_Byte(uchar add);
extern void Set_DS1302_TIME();
extern void DS1302_Init(void);
extern void DS1302_Time_Display(void);
extern uchar DATE_Buf[];
extern uchar Dis_DATE[];
extern uchar DATE[];
extern uchar code ASCII[];
extern uchar code DATE_Write_Add[];		// Ð´¼Ä´æÆ÷
extern uchar code DATE_Read_Add[];		// ¶Á¼Ä´æÆ÷
#endif 