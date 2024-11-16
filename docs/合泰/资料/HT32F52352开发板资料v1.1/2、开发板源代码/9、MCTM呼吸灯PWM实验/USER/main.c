#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "mctm.h"





int main(void)
{
	int i;
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	uart_init(9600);
	mctm_pwm_Init(10000);//10khz
	mctm_pwm(50);
	while(1)
	{    
		
//		for(i=0;i<100;i++)
//		{
//			mctm_pwm(i);
//			delay_ms(10);
//		}
//		for(i=100;i>0;i--)
//		{
//			mctm_pwm(i);
//			delay_ms(10);
//		}
		
		
	}
}

