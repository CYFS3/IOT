#include "ht32.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "lcd.h"
#include "rtc.h"
#include "stdlib.h"


Time_T DateTime, CurTime;

int main(void)
{
	char datebuff[40];
	delay_init();//延时函数初始化	  
	rtc_init();
	uart_init(9600);
	LCD_Init();
	LCD_Clear(WHITE);
	
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_ShowString(30,260,180,20,16,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(30,290,180,20,16,(u8*)"WWW.HOLTEKSUPPORT.COM");
	
	  //检查上电复位标志位
	  if (PWRCU_GetFlagStatus() == PWRCU_FLAG_PWRPOR)
	  {
		LCD_ShowString(0,10,180,20,12,(u8*)"Power On Reset occurred! ");
	  }
	  
	  //读取备份域寄存器 判断RTC是否已经配置
	  if (PWRCU_ReadBackupRegister((PWRCU_BAKREG_Enum) PWRCU_BAKREG_0) != 0xAA55AA55)
	  {
		// RTC 配置
		RTC_Configuration();
		  
		  


		//设置时间
		DateTime.year   = 2020;
		DateTime.month  = 3;
		DateTime.day    = 11;
		DateTime.hour   = 15;
		DateTime.minute = 31;
		DateTime.second = 0;
		  
		if (!AP_Time_Adjust(&DateTime))
		{
		  LCD_ShowString(0,10,180,20,12,(u8*)"Adjust time error!      ");
		  
		  while (1);
		}

		//使能 RTC 
		RTC_Cmd(ENABLE);

		PWRCU_WriteBackupRegister((PWRCU_BAKREG_Enum) PWRCU_BAKREG_0, 0xAA55AA55);
	  }
	  else
	  {
		LCD_ShowString(0,10,180,20,12,(u8*)"RTC has been configured!");
	  }
	  
	  
	  while (1)
	  {
		  
		if(USART_RX_STA&RECFINISH)
		{
			char ch[5];
			if(USART_RX_BUF[4]=='-' && USART_RX_BUF[16]==':')
			{
				//串口发送如下格式更新时间 回车换行结尾
				//2020-03-04 16:27:30
				ch[0]=USART_RX_BUF[0];
				ch[1]=USART_RX_BUF[1];
				ch[2]=USART_RX_BUF[2];
				ch[3]=USART_RX_BUF[3];
				ch[4]=0;
				DateTime.year=atoi(ch);
				
				ch[0]=USART_RX_BUF[5];
				ch[1]=USART_RX_BUF[6];
				ch[2]=0;
				DateTime.month=atoi(ch);
				
				ch[0]=USART_RX_BUF[8];
				ch[1]=USART_RX_BUF[9];
				ch[2]=0;
				DateTime.day=atoi(ch);
				
				ch[0]=USART_RX_BUF[11];
				ch[1]=USART_RX_BUF[12];
				ch[2]=0;
				DateTime.hour=atoi(ch);
				
				ch[0]=USART_RX_BUF[14];
				ch[1]=USART_RX_BUF[15];
				ch[2]=0;
				DateTime.minute=atoi(ch);
				
				ch[0]=USART_RX_BUF[17];
				ch[1]=USART_RX_BUF[18];
				ch[2]=0;
				DateTime.second=atoi(ch);
				
				RTC_Cmd(DISABLE);
				if (!AP_Time_Adjust(&DateTime))
				{
				  LCD_ShowString(0,10,180,20,12,(u8*)"Adjust time error!      ");
				  while (1);
				}
				RTC_Cmd(ENABLE);
			}
			USART_RX_STA=0;
		}			
		
		
		if (CK_SECOND_Flag)
		{
		  CK_SECOND_Flag = 0;
		  //显示当前时间
		  AP_Time_Count(&CurTime);
		  sprintf(datebuff,"%04d-%02d-%02d %02d:%02d:%02d",CurTime.year,
														   CurTime.month,
														   CurTime.day,
														   CurTime.hour,
														   CurTime.minute,
														   CurTime.second);
		 LCD_ShowString(0,30,240,40,16,(u8*)"Current Time:");
		 POINT_COLOR=BLUE;
		 LCD_ShowString(0,50,240,40,16,(u8*)datebuff);
		 POINT_COLOR=BLACK;
		}
	  }

	
}

