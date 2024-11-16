#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "wdt.h"




int main(void)
{
	
	delay_init();//��ʱ������ʼ��	  
	led_init();//��ʼ����LED���ӵ�Ӳ���ӿ�
	key_init();
	
	wdt_Init(WDT_PRESCALER_64,512);//T=1/(32.768k/64)*512=1S�������
	
	
	//ÿ�θ�λʱ��led0��500ms
	led1(0);
	delay_ms(500);
	led1(1);
	
	
	while(1)
	{
		if(getkey())//��key1 ����key2����ʱ����ʼι����ϵͳ���ٸ�λ
		{
			WDT_Feed();
		}
	}
}

