#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "mctm.h"
#include "lcd.h"
#include "bftm.h"
#include "adc.h"


extern unsigned int const pic[];
extern u8 f1s;

int main(void)
{
	u16 temp=0;
	u16 backlight=100;
	char mchar[30];
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	uart_init(9600);
	LCD_Init();
	mctm_pwm_Init(20000);
	
	adc_Init();
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
	POINT_COLOR=RED;		
	LCD_ShowString(0,80,240,20,12,(u8*)"Connect the LIGHT and ADC of P5");
	POINT_COLOR=BLACK;	
	
	while(1)
	{		

		temp=getlight();
		if(temp>backlight)backlight++;
		else if(temp<backlight)backlight--;
		mctm_pwm(backlight);
		sprintf(mchar,"LCD backlight:%3d%%",temp);
	    LCD_ShowString(0,100,180,20,16,(u8*)mchar);
		
		delay_ms(50);
	}
}

