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
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	beep_init();//初始化与蜂鸣器连接的硬件接口
	key_init();//按键相关初始化
	
	
	uart_init(9600);//初始化串口波特率9600 8 N 1
	

	printf("\r\n合泰单片机开发板\r\n");
	printf("HT32M0+ 开发板\r\n");
	printf("EASEAR\r\n");
	printf("www.holteksupport.com\r\n");
	
	
	printf("请输入信息 以回车换行结尾\r\n");
	while(1)
	{
		if(USART_RX_STA&RECFINISH)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n接收到如下信息:\r\n");
			for(i=0;i<len;i++)
			{
				USART_SendData(HT_USART0, USART_RX_BUF[i]);//向串口1发送数据
				while(USART_GetFlagStatus(HT_USART0,USART_FLAG_TXC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}

		
		
		keyscan();
		if(KeySta==KEYPRESS)//按键按下去时
		{
			switch(KeyVal)
			{
				case 0x01:
						beeptime=5;
						printf("\r\n按键1按下\r\n");
				break;
				case 0x02:
						beeptime=5;
						printf("\r\n按键2按下\r\n");
				break;
			}
			KeySta=0;
		}
		else if(KeySta==KEYRELEASE)//按键释放
		{
			switch(KeyVal)
			{
				case 0x01:printf("\r\n按键1释放\r\n");break;
				case 0x02:printf("\r\n按键2释放\r\n");break;
			}
			KeySta=0;
		}
		else if(KeySta==KEYLONG)//长按键
		{
			switch(KeyVal)
			{
				case 0x01:printf("\r\n长按按键1\r\n");break;
				case 0x02:printf("\r\n长按按键2\r\n");break;
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

