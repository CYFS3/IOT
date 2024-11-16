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
#define  FLASH_ReadWordData(addr)   (*((u32 volatile *) (addr)))

#define StartAddress    (1024 * 100)//起始地址必须是1024的倍数   最大1024*128



extern unsigned int const pic[];
extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;




int main(void)
{
	u32 readdata1;
	u32 readdata2;
	
	char rdata[40];
	delay_init();//延时函数初始化	  
	key_init();
	uart_init(9600);
	LCD_Init();
	
	
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
	POINT_COLOR=RED;		
	LCD_ShowString(0,0,240,20,16,(u8*)"Press button 1 to write data");
	LCD_ShowString(0,20,240,20,16,(u8*)"Press button 2 to read  data");
	POINT_COLOR=BLACK;	
	
	while(1)
	{		
		keyscan();
		if (KeySta==KEYPRESS)
		{
			if(KeyVal==1)
			{
				LCD_ShowString(0,60,240,16,16,(u8*)"                          ");
				LCD_ShowString(0,40,200,16,16,(u8*)"Writing....    ");
				
				FLASH_ErasePage(StartAddress);//擦除page
				FLASH_ProgramWordData(StartAddress,12345);
				FLASH_ProgramWordData(StartAddress+4,67890);
				
				LCD_ShowString(0,40,200,16,16,(u8*)"Write Finished!");
			}
			else if(KeyVal==2)
			{
				LCD_ShowString(0,40,200,16,16,(u8*)"Reading....    ");
				
				readdata1=FLASH_ReadWordData(StartAddress);
				readdata2=FLASH_ReadWordData(StartAddress+4);
				
				sprintf(rdata,"num1:%d,num2:%d",readdata1,readdata2);
				
				LCD_ShowString(0,40,200,16,16,(u8*)"Readed data:   ");
				POINT_COLOR=BLUE;
				LCD_ShowString(0,60,240,16,16,(u8*)rdata);
				POINT_COLOR=BLACK;
			}
			KeySta=0;
		}
		
		delay_ms(50);
	}
}

