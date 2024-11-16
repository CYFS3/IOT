#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "mctm.h"
#include "lcd.h"
#include "bftm.h"
#include "stdlib.h"



extern unsigned int const pic[];
extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

int main(void)
{
	char *p;
	delay_init();//延时函数初始化	  
	key_init();
	uart_init(9600);
	LCD_Init();
	
	
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
		
	p=malloc(2048);
	if(p!=NULL)
	{
		sprintf(p,"Memory Management    ");
		LCD_ShowString(0,30,180,20,16,(u8*)p);
	}
	else
	{
		LCD_ShowString(0,30,180,20,16,(u8*)"Memory Management ERR");
	}
	free(p);
	
	
	while(1)
	{		

	}
}

