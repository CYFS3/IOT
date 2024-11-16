#include "ht32.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "lcd.h"
#include "rtc.h"
#include "stdio.h"


Time_T DateTime, CurTime;

int main(void)
{
	u32 wdata=12345;
	u32 rdata=0;
	
	char buff[10];
	
	delay_init();//延时函数初始化	
    key_init();	
	rtc_init();
	uart_init(9600);
	LCD_Init();
	LCD_Clear(WHITE);

	
	POINT_COLOR=RED;		
	LCD_ShowString(0,0,240,20,16,(u8*)"Press button 1 to write data");
	LCD_ShowString(0,20,240,20,16,(u8*)"Press button 2 to read  data");
	POINT_COLOR=BLACK;
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");

	//检查上电复位标志位
	if (PWRCU_GetFlagStatus() == PWRCU_FLAG_PWRPOR)
	{
		LCD_ShowString(0,10,180,20,12,(u8*)"Power On Reset occurred! ");
	}





	while (1)
	{
		keyscan();
		if (KeySta==KEYPRESS)
		{
			if(KeyVal==1)
			{
				LCD_ShowString(0,60,240,16,16,(u8*)"                          ");
				LCD_ShowString(0,40,200,16,16,(u8*)"Writing....    ");
				PWRCU_WriteBackupRegister((PWRCU_BAKREG_Enum) PWRCU_BAKREG_2, wdata);//写入备份域寄存器 
				LCD_ShowString(0,40,200,16,16,(u8*)"Write Finished!");
			}
			else if(KeyVal==2)
			{
				LCD_ShowString(0,40,200,16,16,(u8*)"Reading....    ");
				rdata=PWRCU_ReadBackupRegister((PWRCU_BAKREG_Enum) PWRCU_BAKREG_2);	//读取备份域寄存器 
				LCD_ShowString(0,40,200,16,16,(u8*)"Readed data:   ");
				sprintf(buff,"%d",rdata);
				POINT_COLOR=BLUE;
				LCD_ShowString(0,60,240,16,16,(u8*)buff);
				POINT_COLOR=BLACK;
			}
			KeySta=0;
		}
		
		delay_ms(50);
	}

	
}

