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
//8bit 16k wav ��Ƶ�ļ�
//������ʽ����Ч������




extern unsigned int const pic[];
extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

int main(void)
{

	u8 res=0;
	delay_init();//��ʱ������ʼ��	  
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

	
	res=f_mount(fs[0],"0:",1);//����SD�� 
	
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
	
 	res=f_mount(fs[1],"1:",1);//����FLASH.	
	if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
	{
		LCD_ShowString(0,80,200,16,16,(u8*)"Flash Disk Formatting...");	    //��ʽ��FLASH
		res=f_mkfs("1:",1,4096);                                                //��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			  f_setlabel((const TCHAR *)"1:EASEAR");	                            //����Flash���̵�����
			  LCD_ShowString(0,80,200,16,16,(u8*)"Flash Disk Format Finish");	//��ʽ�����
		}else LCD_ShowString(0,80,200,16,16,(u8*)"Flash Disk Format Error ");	//��ʽ��ʧ��
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
		
		Show_Str(0,100,200,16,(u8*)"�ͺվ�Լ.wav        ",16,0);
		Playwav((char*)"0:WAV/�ͺվ�Լ.wav");		
		
		Show_Str(0,100,200,16,(u8*)"��ȫ������������.wav",16,0);
		Playwav((char*)"0:WAV/��ȫ������������.wav");		

		delay_ms(50);
	}
}

