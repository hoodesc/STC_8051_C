#ifndef __DS18B20_H__
#define __DS18B20_H__

extern uchar code number[];
extern uchar c[];
extern bit DS18B20_flag;
extern void DS18B20_Write(uchar udata);
extern uchar DS18B20_Read();
extern void DS18B20_Init();
extern void compute(uchar lsb,uchar msb);
extern void DS18B20_Temperature_Read();


#endif