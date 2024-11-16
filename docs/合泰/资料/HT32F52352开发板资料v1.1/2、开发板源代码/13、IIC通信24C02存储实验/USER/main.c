#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "mctm.h"
#include "lcd.h"
#include "bftm.h"
#include "24cxx.h"

extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

int main(void)
{
	u8 len=0;
	char wdata[40];
	char rdata[40];
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	key_init();
	uart_init(9600);
	LCD_Init();
	
	AT24CXX_Init();
	
	
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
	POINT_COLOR=RED;		
	LCD_ShowString(0,0,240,20,16,(u8*)"Press button 1 to write data");
	LCD_ShowString(0,20,240,20,16,(u8*)"Press button 2 to read  data");
	POINT_COLOR=BLACK;	
	
	
	sprintf(wdata,"\"Welcome to Holtek MCU\" by:EASEAR");
	len=strlen(wdata)+1;
	while(1)
	{		
		keyscan();
		if (KeySta==KEYPRESS)
		{
			if(KeyVal==1)
			{
				LCD_ShowString(0,60,240,16,16,(u8*)"                          ");
				LCD_ShowString(0,40,200,16,16,(u8*)"Writing....    ");
				AT24CXX_Write(0,(u8*)wdata,len);
				LCD_ShowString(0,40,200,16,16,(u8*)"Write Finished!");
			}
			else if(KeyVal==2)
			{
				LCD_ShowString(0,40,200,16,16,(u8*)"Reading....    ");
				AT24CXX_Read(0,(u8*)rdata,len);
				LCD_ShowString(0,40,200,16,16,(u8*)"Readed data:   ");
				POINT_COLOR=BLUE;
				LCD_ShowString(0,60,240,16,12,(u8*)rdata);
				POINT_COLOR=BLACK;
			}
			KeySta=0;
		}
		
		delay_ms(50);
	}
}

