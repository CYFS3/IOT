#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "wdt.h"




int main(void)
{
	
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	key_init();
	
	wdt_Init(WDT_PRESCALER_64,512);//T=1/(32.768k/64)*512=1S溢出周期
	
	
	//每次复位时，led0亮500ms
	led1(0);
	delay_ms(500);
	led1(1);
	
	
	while(1)
	{
		if(getkey())//当key1 或者key2按下时，开始喂狗，系统不再复位
		{
			WDT_Feed();
		}
	}
}

