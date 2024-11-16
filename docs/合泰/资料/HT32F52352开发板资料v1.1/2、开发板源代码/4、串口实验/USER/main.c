#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"



extern u8 KeyVal;
extern u8 KeySta;


u32 beeptime=0;

u8 len=0;
u8 i;
int main(void)
{
	delay_init();//��ʱ������ʼ��	  
	led_init();//��ʼ����LED���ӵ�Ӳ���ӿ�
	beep_init();//��ʼ������������ӵ�Ӳ���ӿ�
	key_init();//������س�ʼ��
	
	
	uart_init(9600);//��ʼ�����ڲ�����9600 8 N 1
	

	printf("\r\n��̩��Ƭ��������\r\n");
	printf("HT32M0+ ������\r\n");
	printf("EASEAR\r\n");
	printf("www.holteksupport.com\r\n");
	
	
	printf("��������Ϣ �Իس����н�β\r\n");
	while(1)
	{
		if(USART_RX_STA&RECFINISH)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n���յ�������Ϣ:\r\n");
			for(i=0;i<len;i++)
			{
				USART_SendData(HT_USART0, USART_RX_BUF[i]);//�򴮿�1��������
				while(USART_GetFlagStatus(HT_USART0,USART_FLAG_TXC)!=SET);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}

		
		
		keyscan();
		if(KeySta==KEYPRESS)//��������ȥʱ
		{
			switch(KeyVal)
			{
				case 0x01:
						beeptime=5;
						printf("\r\n����1����\r\n");
				break;
				case 0x02:
						beeptime=5;
						printf("\r\n����2����\r\n");
				break;
			}
			KeySta=0;
		}
		else if(KeySta==KEYRELEASE)//�����ͷ�
		{
			switch(KeyVal)
			{
				case 0x01:printf("\r\n����1�ͷ�\r\n");break;
				case 0x02:printf("\r\n����2�ͷ�\r\n");break;
			}
			KeySta=0;
		}
		else if(KeySta==KEYLONG)//������
		{
			switch(KeyVal)
			{
				case 0x01:printf("\r\n��������1\r\n");break;
				case 0x02:printf("\r\n��������2\r\n");break;
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

