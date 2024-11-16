#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "mctm.h"
#include "lcd.h"
#include "bftm.h"


extern unsigned int const pic[];
extern u8 f1s;

int main(void)
{
	delay_init();//��ʱ������ʼ��	  
	led_init();//��ʼ����LED���ӵ�Ӳ���ӿ�
	uart_init(9600);
	LCD_Init();
	
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	LCD_DispPicture(20,100,200,59,(u16*)pic);
	
	LCD_ShowString(30,260,180,20,16,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(30,290,180,20,16,(u8*)"WWW.HOLTEKSUPPORT.COM");
	
	while(1);
}

