#include "ht32.h"
#include "delay.h"
#include "led.h"




int main(void)
{
	delay_init();//��ʱ������ʼ��	  
	led_init();//��ʼ����LED���ӵ�Ӳ���ӿ�
	
	while(1)
	{
		led1(1);  
		delay_ms(500); 
		led1(0); 
		delay_ms(400); 
		led2(0);   
		delay_ms(50); 
		led2(1);     
		delay_ms(50);  
		
	}
}

