#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "mctm.h"
#include "lcd.h"
#include "bftm.h"
#include "sd.h"


extern unsigned int const pic[];
extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

int main(void)
{
	u8 sta=0;
	int i;
	u8 buff[512];
	delay_init();//延时函数初始化	  
	key_init();
	uart_init(9600);
	LCD_Init();
	
	
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
	
	while(1)	//检测SD
	{
		long long charsize=0;
		sta=SD_Initialize();
		
		if(sta==0)//检测完成
		{
			charsize=SD_GetSectorCount();
			LCD_ShowString(0,40,200,16,16,(u8*)"SD Card OK!       ");
			LCD_ShowxNum(0,60,charsize>>11,4,16,1);
			LCD_ShowString(50,60,200,16,16,(u8*)"MB");
			break;
		}
		else
		{
			LCD_ShowString(0,40,200,16,16,(u8*)"SD Card Check Failed!");
		}
		delay_ms(500);
		LCD_ShowString(0,40,200,16,16,(u8*)"                    ");
		
	}
	
	
	POINT_COLOR=RED;		
	LCD_ShowString(0,0,240,20,12,(u8*)"Press button 1 Read Data to USART");
	POINT_COLOR=BLACK;	
	
	
	while(1)
	{		
		keyscan();
		if (KeySta==KEYPRESS)
		{
			if(KeyVal==1)
			{
				if(SD_ReadDisk(buff,0,1)==0)	//读取0扇区的内容
				{	
					LCD_ShowString(0,80,200,16,16,(u8*)"USART Sending Data...");
					printf("SECTOR 0 DATA:\r\n");
					for(i=0;i<512;i++)printf("%02x ",buff[i]);//打印0扇区数据    	   
					printf("\r\nDATA ENDED\r\n");
					LCD_ShowString(0,80,200,16,16,(u8*)"USART Send Data Over!");
				}
			}

			KeySta=0;
		}
		
		delay_ms(50);
	}
}

