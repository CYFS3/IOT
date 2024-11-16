#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"


extern u8 KeyVal;
extern u8 KeySta;


u8 ledsta1=0xff;
u8 ledsta2=0xff;

u32 beeptime=0;
int main(void)
{
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	beep_init();//初始化与蜂鸣器连接的硬件接口
	key_init();//按键相关初始化
	while(1)
	{
		keyscan();
		if(KeySta==KEYPRESS)//按键按下去时
		{
			switch(KeyVal)
			{
				case 0x01:
						beeptime=5;
						ledsta1=~ledsta1;
						led1(ledsta1);
				break;
				case 0x02:
						beeptime=5;
						ledsta2=~ledsta2;
						led2(ledsta2);
				break;
			}
			KeySta=0;
		}
		else if(KeySta==KEYRELEASE)//按键释放
		{
			switch(KeyVal)
			{
				case 0x01:break;
				case 0x02:break;
			}
			KeySta=0;
		}
		else if(KeySta==KEYLONG)//长按键
		{
			switch(KeyVal)
			{
				case 0x01:break;
				case 0x02:break;
			}
			KeySta=0;
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
		
		delay_ms(10); //延时100ms
		
	}
}

