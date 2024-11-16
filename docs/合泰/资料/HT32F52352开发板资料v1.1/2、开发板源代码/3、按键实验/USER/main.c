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
	delay_init();//��ʱ������ʼ��	  
	led_init();//��ʼ����LED���ӵ�Ӳ���ӿ�
	beep_init();//��ʼ������������ӵ�Ӳ���ӿ�
	key_init();//������س�ʼ��
	while(1)
	{
		keyscan();
		if(KeySta==KEYPRESS)//��������ȥʱ
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
		else if(KeySta==KEYRELEASE)//�����ͷ�
		{
			switch(KeyVal)
			{
				case 0x01:break;
				case 0x02:break;
			}
			KeySta=0;
		}
		else if(KeySta==KEYLONG)//������
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
		
		delay_ms(10); //��ʱ100ms
		
	}
}

