#include <stc15.h>
#include "mytype.h"
#include "LCD12864.h"
#include "ds1302.h"
#include "delay.h"
#include "uart.h"
#include "nrf24l01_rx.h"


void INIT(void)
{
		P2M0 = 0xFF;
		P2M1 = 0x00;		
		P4M0 = 0x00;
		P4M1 = 0x00;
		P3M0 = 0x00;
		P3M1 = 0x00;

		NRF24L01_Init();			//	NRF24L01无线模块初始化					
//		DS1302_Init();				//	DS1302时钟模块初始化
		LCD_Init();						//	LCD12864液晶初始化
}