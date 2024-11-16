#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "wav.h"
#include "key.h"
#include "usart.h"
#include "stdlib.h"
#include "lcd.h"
#include "flash.h"
#include "sd.h"
#include "ff.h"  
#include "exfuns.h"
#include "TEXT.h"
//8bit 16k wav 音频文件
//其他形式播放效果不好




extern unsigned int const pic[];
extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

int main(void)
{

	u8 res=0;
	delay_init();//延时函数初始化	  
	key_init();
	uart_init(9600);
	LCD_Init();
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
	
	audio_init();
	SPI_Flash_Init();
	SD_Initialize();

	exfuns_init();

	
	res=f_mount(fs[0],"0:",1);//挂载SD卡 
	
	if(res==0)
	{
		LCD_ShowString(0,60,200,16,16,(u8*)"SD FATFS OK!");	
	}
	else
	{
		LCD_ShowString(0,60,200,16,16,(u8*)"SD CARD ERR!");	
		while(1);
	}
	if(font_init())
	{
		update_font(0,120,16);
	}
	
 	res=f_mount(fs[1],"1:",1);//挂载FLASH.	
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		LCD_ShowString(0,80,200,16,16,(u8*)"Flash Disk Formatting...");	    //格式化FLASH
		res=f_mkfs("1:",1,4096);                                                //格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			  f_setlabel((const TCHAR *)"1:EASEAR");	                            //设置Flash磁盘的名字
			  LCD_ShowString(0,80,200,16,16,(u8*)"Flash Disk Format Finish");	//格式化完成
		}else LCD_ShowString(0,80,200,16,16,(u8*)"Flash Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}
	else if(res==0)
	{
		LCD_ShowString(0,80,200,16,16,(u8*)"SPI FLASH FATFS OK!");	 
	}		
	
	delay_ms(1000);
	
    LCD_ShowString(0,20,240,16,12,(u8*)"Make sure the SD card has a wav file");
	

	
	POINT_COLOR=BLUE;
	while(1)
	{	
		
		LCD_ShowString(0,100,200,16,16,(u8*)"ht32.wav            ");	
		Playwav((char*)"0:WAV/ht32.wav");
		
		Show_Str(0,100,200,16,(u8*)"巴赫旧约.wav        ",16,0);
		Playwav((char*)"0:WAV/巴赫旧约.wav");		
		
		Show_Str(0,100,200,16,(u8*)"向全世界宣布爱你.wav",16,0);
		Playwav((char*)"0:WAV/向全世界宣布爱你.wav");		

		delay_ms(50);
	}
}

