#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "bftm.h"




int main(void)
{
	
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	key_init();
	bftm_Init(2000);//2ms

	
	
	while(1)
	{
	}
}

