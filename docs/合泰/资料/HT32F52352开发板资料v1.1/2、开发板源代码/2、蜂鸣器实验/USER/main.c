#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "beep.h"




int main(void)
{
	delay_init();//��ʱ������ʼ��	  
	led_init();//��ʼ����LED���ӵ�Ӳ���ӿ�
	beep_init();//��ʼ������������ӵ�Ӳ���ӿ�
	while(1)
	{
		led1(0);     //����LED1
		beep(1);     
		delay_ms(100); //��ʱ100ms
		led1(1);     //Ϩ��LED2
		beep(0);
		delay_ms(900); //��ʱ900ms
		
	}
}

