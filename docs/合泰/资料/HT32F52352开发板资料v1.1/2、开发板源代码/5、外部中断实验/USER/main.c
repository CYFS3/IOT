#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "exit.h"




extern u8 button1_exit;
extern u8 button2_exit;
u32 beeptime=0;

int main(void)
{
	u8 led1sta=0xff;
	u8 led2sta=0xff;
	
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	beep_init();//初始化与蜂鸣器连接的硬件接口
	exit_init();
	
	while(1)
	{
		
		if(button1_exit)
		{
			led1sta=~led1sta;
			led1(led1sta);
			beeptime=5;
			button1_exit=0;
		}
		
		if(button2_exit)
		{
			led2sta=~led2sta;
			led2(led2sta);
			beeptime=5;
			button2_exit=0;
		}
		
		if(beeptime>0)
		{
			beeptime--;
			beep(1);
		}
		else
		{
			beep(0);
		}
		
		delay_ms(10); //延时10ms
		
	}
}

