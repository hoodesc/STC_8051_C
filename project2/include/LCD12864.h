#ifndef __LCD12864_H__
#define __LCD12864_H__

extern uchar code WenDu[];
extern uchar code QiTi[];
extern uchar code JingGao[];
extern uchar code ZhengChang[];
extern uchar code CMCC[];
extern uchar code CUCC[];
extern uchar code No_service[];


extern void LCD_Write_Dat(uchar dat);
extern void LCD_Write_Com(uchar com);
extern void LCD_Write_Byte(uchar dat);
extern void LCD_XY(uchar x,y);
extern void LCD12864_Init();

#endif