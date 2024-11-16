#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "beep.h"
#include "lcd.h"
#include "bftm.h"
#include "ir.h"


extern u16 const pic[];
extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

u16 beeptime=0;

int main(void)
{
	char keyname[10];
	delay_init();
	beep_init();
	LCD_Init();
	ir_Init();
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	POINT_COLOR=RED;
	LCD_ShowString(35,275,240,20,12,(u8*)"Connect the WIRE and IR of P7");
	POINT_COLOR=BLACK;
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
			
	
		
	
	
	LCD_DispPicture(45,60,150,137,(u16*)pic);
	
	
	while(1)
	{		
			if(IR_OK)
			{
				switch(IR_code)
				{
					case 0x45:sprintf(keyname,"ON/OFF");break;
					case 0x47:sprintf(keyname,"MENU  ");break;
					case 0x44:sprintf(keyname,"TEST  ");break;
					case 0x40:sprintf(keyname,"+     ");break;
					case 0x43:sprintf(keyname,"BACK  ");break;
					case 0x07:sprintf(keyname,"LEFT  ");break;
					case 0x15:sprintf(keyname,"PAUSE ");break;
					case 0x09:sprintf(keyname,"RIGHT ");break;
					case 0x16:sprintf(keyname,"0     ");break;
					case 0x19:sprintf(keyname,"-     ");break;
					case 0x0d:sprintf(keyname,"CLEAR ");break;
					case 0x0c:sprintf(keyname,"1     ");break;
					case 0x18:sprintf(keyname,"2     ");break;
					case 0x5e:sprintf(keyname,"3     ");break;
					case 0x08:sprintf(keyname,"4     ");break;
					case 0x1c:sprintf(keyname,"5     ");break;
					case 0x5a:sprintf(keyname,"6     ");break;
					case 0x42:sprintf(keyname,"7     ");break;
					case 0x52:sprintf(keyname,"8     ");break;
					case 0x4a:sprintf(keyname,"9     ");break;
					
				}
				beeptime=5;
				LCD_ShowString(55,220,240,20,16,(u8*)"IR key is:");
				POINT_COLOR=BLUE;
			    LCD_ShowString(145,220,240,20,16,(u8*)keyname);
				POINT_COLOR=BLACK;
				IR_OK=0;
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
		
		delay_ms(10);
		
	}
}

