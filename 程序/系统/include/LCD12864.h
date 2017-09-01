#ifndef __LCD12864_H__
#define __LCD12864_H__

extern uchar code table[];  
extern uchar code table1[];
extern uchar code PHOTO[]; 
	
extern void Photo(uchar *pPic);
extern void LCD_Write_data(uchar dat);
extern void LCD_Write_com(uchar com);
extern void LCD_Init();
extern void LCD_XY(uchar x,y);
extern void LCD_Display(uchar x,uchar y,uchar *pdat);

#endif