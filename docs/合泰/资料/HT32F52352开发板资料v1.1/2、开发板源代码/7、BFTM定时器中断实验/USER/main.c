#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "bftm.h"




int main(void)
{
	
	delay_init();//��ʱ������ʼ��	  
	led_init();//��ʼ����LED���ӵ�Ӳ���ӿ�
	key_init();
	bftm_Init(2000);//2ms

	
	
	while(1)
	{
	}
}

