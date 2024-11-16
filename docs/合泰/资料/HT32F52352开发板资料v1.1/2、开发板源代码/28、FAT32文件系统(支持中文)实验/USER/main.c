#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "stdlib.h"
#include "lcd.h"
#include "flash.h"
#include "sd.h"
#include "ff.h"  
#include "exfuns.h"
#include "text.h"



extern unsigned int const pic[];
extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

int main(void)
{
	FILINFO fno;
	FIL *txtfile;
	u32 bw;

	u8 res=0;
	delay_init();//延时函数初始化	  
	key_init();
	uart_init(9600);
	LCD_Init();
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);

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
		update_font(20,110,16);
	}
 	POINT_COLOR=RED;  
	
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
	
	LCD_Clear(WHITE);
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	

	
	Show_Str(0,0,200,16,(u8*)"按按键1写文件:",16,0);	
	Show_Str(0,20,200,16,(u8*)"按按键2读文件:",16,0);	
	POINT_COLOR=BLACK;
	while(1)
	{		
		keyscan();
		if (KeySta==KEYPRESS)
		{
			if(KeyVal==1)
			{
				txtfile=malloc(sizeof(FIL));
				if(f_stat( "0:合泰.txt",&fno)==0)//如果已经存在则删除
				{
					f_unlink("0:合泰.txt");
					Show_Str(20,150,200,16,(u8*)"合泰.txt  已经删除",16,0);
					delay_ms(1000);
				}
				
				
				res=f_open(txtfile,"0:合泰.txt",FA_CREATE_ALWAYS | FA_WRITE);
				if(res==0)
				{
					f_write(txtfile,"欢迎使用合泰开发板",18,&bw);
					f_close(txtfile);
					Show_Str(20,150,200,16,(u8*)"合泰.txt  创建成功",16,0);
					
				}
				else
				{
					Show_Str(20,150,200,16,(u8*)"创建文件错误！     ",16,0);
				}
					
				
				free(txtfile);
			}
			else if(KeyVal==2)
			{
				txtfile=malloc(sizeof(FIL));
				if(f_stat( "0:合泰.txt",&fno)==0)//如果已经存
				{
					char *buff=malloc(fno.fsize);//分配一个文件大小的内存空间，注意文件大小及分配结果
					f_open(txtfile, "0:合泰.txt", FA_OPEN_EXISTING | FA_READ);
					if(f_read(txtfile, buff,fno.fsize, &bw)==0)
					{
						Show_Str(20,150,200,16,(u8*)"合泰.txt  读取成功",16,0);
						Show_Str(20,180,200,16,(u8*)"文本内容是:",16,0);
						POINT_COLOR=BLUE;
						Show_Str(20,200,200,16,(u8*)buff,16,0);
						POINT_COLOR=BLACK;
					}
					else
					{
						Show_Str(20,150,200,16,(u8*)"文件读取错误！     ",16,0);
					}
					
					free(buff);
					
				}
				else
				{
					Show_Str(20,150,200,16,(u8*)"文件不存在！       ",16,0);
				
				}
				free(txtfile);
			
			}

			KeySta=0;
		}
		
		delay_ms(50);
	}
}

