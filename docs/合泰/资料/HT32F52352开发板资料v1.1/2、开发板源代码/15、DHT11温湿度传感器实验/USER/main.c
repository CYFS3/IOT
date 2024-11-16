#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "dht11.h"



int main(void)
{
	u8 cnt120ms=0;
	
	u8 temperature;  	    
	u8 humidity;    
	
	
	delay_init();
	LCD_Init();
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	POINT_COLOR=RED;
	LCD_ShowString(35,275,240,20,12,(u8*)"Connect the WIRE and DHT11 of P7");
	POINT_COLOR=BLACK;
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	while(DHT11_Init())
	{
		LCD_ShowString(60,130,200,16,16,(u8*)"DHT11 Error");
		delay_ms(200);
		LCD_ShowString(60,130,200,16,16,(u8*)"           ");
 		delay_ms(200);
	}
	
	LCD_ShowString(40,100,200,16,16,(u8*)"Temperature:   C");	
	LCD_ShowString(40,140,200,16,16,(u8*)"Humidity:   %");	
	
	while(1)
	{		
		if(++cnt120ms>12)//120ms∂¡»°“ª¥Œ
		{
			cnt120ms=0;
			DHT11_Read_Data(&temperature,&humidity);	
            POINT_COLOR=BLUE;			
			LCD_ShowNum(140,100,temperature,2,16); 
			LCD_ShowNum(116,140,humidity,2,16);
			POINT_COLOR=BLACK;
		}			
		delay_ms(10);
		
	}
}

