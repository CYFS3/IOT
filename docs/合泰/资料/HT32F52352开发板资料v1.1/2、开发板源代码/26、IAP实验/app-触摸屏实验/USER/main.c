#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "mctm.h"
#include "lcd.h"
#include "touch.h"


extern unsigned int const pic[];
extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

u16 const colortab[11]={WHITE,BLACK,BLUE,GRAYBLUE,RED,MAGENTA,GREEN,CYAN,YELLOW,BROWN,GRAY};

#define BOOT_MODE_AP                  (0x55AAFAF0)
#define BOOT_MODE_IAP                 (0x55AAFAF5)
#define BOOT_MODE                     ((*((u32 volatile *) (HT_SRAM_BASE+0x4000-4))))



int main(void)
{
    u8 i;
	u16 pencolour=BLACK;
	NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 1024*20);
	
	
	delay_init();//延时函数初始化	  
	key_init();
	uart_init(9600);
	LCD_Init();
	
	TP_Init();
	
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
	
	POINT_COLOR=RED;	
    LCD_ShowString(0,0,240,20,16,(u8*) "When the touch is not accurate    ");	
	LCD_ShowString(0,20,240,20,16,(u8*)"please press button1 calibrate    ");
	LCD_ShowString(0,40,240,20,16,(u8*)"please press button2 to bootloader");
	POINT_COLOR=BLACK;	
	
	delay_ms(2000);
	
	LCD_Clear(WHITE); 
	POINT_COLOR=BLACK;
	LCD_ShowString(224,0,200,16,16,(u8*)"R");//显示清屏区域
			

	for(i=0;i<11;i++)
	{
		LCD_Fill(i*20,0,i*20+20,20,colortab[i]);	
	}		

	while(1)
	{		
		keyscan();
		
		TP_Scan(0); 		 
		if(TouchSta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(TouchX<lcddev.width&&TouchY<lcddev.height)
			{	
				if(TouchX>220&&TouchY<20)
				{
					LCD_Clear(WHITE); 
					POINT_COLOR=BLACK;
					LCD_ShowString(224,0,200,16,16,(u8*)"R");//显示清屏区域
						for(i=0;i<11;i++)
						LCD_Fill(i*20,0,i*20+20,20,colortab[i]);	
				}
				else if(TouchY<20)
				{
					for(i=0;i<11;i++)
					{
						if(TouchX<=i*20+20)break;
					}
					pencolour=colortab[i];
				
				}
				else if(TouchY>20)
				{
					TP_Draw_Big_Point(TouchX,TouchY,pencolour);		//画图	  
				}

				
			}
		}
		else
		{
			delay_ms(10);
		}
		
		if (KeySta==KEYPRESS)
		{
			if(KeyVal==1)
			{
				LCD_Clear(WHITE);//清屏
				TP_Adjust();  //屏幕校准 
				TP_Save_Adjdata();
				LCD_Clear(WHITE);//清屏   
				POINT_COLOR=BLUE;//设置字体为蓝色 
				LCD_ShowString(lcddev.width-24,0,200,16,16,(u8*)"RST");//显示清屏区域
				POINT_COLOR=RED;//设置画笔蓝色				
			}
			else if(KeyVal==2)
			{
				NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 0);
				BOOT_MODE=BOOT_MODE_IAP;
				NVIC_SystemReset();
			
			}
			KeySta=0;
		}
		
		
	}
}



