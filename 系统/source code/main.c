#include <stc15.h>
#include "mytype.h"
#include "LCD12864.h"
#include "ds1302.h"
#include "delay.h"
#include "uart.h"
#include "nrf24l01_rx.h"



void main() 
{		
//		uchar temp = 0;
//		P2M0 = 0xFF;
//		P2M1 = 0x00;
		NRF24L01_Init();
		Uart_Init();							// ´®¿Ú³õÊ¼»¯				
//		DS1302_Init();
//		LCD_Init();
		DelayMs(1000);
		while(1)
		{
//			DS1302_Time_Display();
			SetRX_Mode();			
			Uart_print();
			DelayMs(100);
//			NRF24L01_RxPacket(RX_DAT);
//			LCD_XY(1,0);
//LCD_Write_data('a');			
		}
}
