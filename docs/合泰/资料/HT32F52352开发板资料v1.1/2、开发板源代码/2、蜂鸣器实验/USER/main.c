#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "beep.h"




int main(void)
{
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	beep_init();//初始化与蜂鸣器连接的硬件接口
	while(1)
	{
		led1(0);     //点亮LED1
		beep(1);     
		delay_ms(100); //延时100ms
		led1(1);     //熄灭LED2
		beep(0);
		delay_ms(900); //延时900ms
		
	}
}

