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



extern unsigned int const pic[];
extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

int main(void)
{
	FILINFO fno;
	FIL *txtfile;
	u32 bw;
	u32 totalrom,freerom;
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
	
	
	POINT_COLOR=RED;		
	LCD_ShowString(0,0,240,20,16,(u8*)"Press button 1 to write file");
	LCD_ShowString(0,20,240,20,16,(u8*)"Press button 2 to read  file");
	POINT_COLOR=BLACK;
	
	
	SPI_Flash_Init();
	SD_Initialize();

	exfuns_init();
	
	
	res=f_mount(fs[0],"0:",1);//挂载SD卡 
	if(res==0)
	{
		exf_getfree((u8*)"0",&totalrom,&freerom);	//得到SD卡的总容量和剩余容量 
		LCD_ShowString(0,60,200,16,16,(u8*)"SD FATFS OK!");	 
		LCD_ShowString(0,80,200,16,16,(u8*)"SD Total Size:     MB");	 
		LCD_ShowString(0,100,200,16,16,(u8*)"SD  Free Size:     MB"); 	    
		LCD_ShowNum(112,80,totalrom>>10,5,16);				//显示SD卡总容量 MB
		LCD_ShowNum(112,100,freerom>>10,5,16);					//显示SD卡剩余容量 MB	
	}
	else
	{
		LCD_ShowString(0,60,200,16,16,(u8*)"SD CARD ERR!");	
		while(1);
	}
	
 	res=f_mount(fs[1],"1:",1);//挂载FLASH.	
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		LCD_ShowString(0,140,200,16,16,(u8*)"Flash Disk Formatting...");	    //格式化FLASH
		res=f_mkfs("1:",1,4096);                                                //格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:EASEAR");	                            //设置Flash磁盘的名字
			LCD_ShowString(0,140,200,16,16,(u8*)"Flash Disk Format Finish");	//格式化完成
		}else LCD_ShowString(0,140,200,16,16,(u8*)"Flash Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}
	else if(res==0)
	{
		LCD_ShowString(0,120,200,16,16,(u8*)"SPI FLASH FATFS OK!");	 
	}		

	
	while(1)
	{		
		keyscan();
		if (KeySta==KEYPRESS)
		{
			if(KeyVal==1)
			{
				txtfile=malloc(sizeof(FIL));
				if(f_stat( "0:easear.txt",&fno)==0)//如果已经存在则删除
				{
					f_unlink("0:easear.txt");
					LCD_ShowString(0,150,200,16,12,(u8*)"easear.txt  already delete");
					delay_ms(1000);
				}
				
				
				res=f_open(txtfile,"0:easear.txt",FA_CREATE_ALWAYS | FA_WRITE);
				if(res==0)
				{
					f_write(txtfile,"welcome to use EASEAR HT32",26,&bw);
					f_close(txtfile);
					LCD_ShowString(0,150,200,16,12,(u8*)"Creart easear.txt success!");	
					
				}
				else
				{
					LCD_ShowString(0,150,200,16,12,(u8*)"Creart easear.txt err!    ");
				}
					
				
				free(txtfile);
			}
			else if(KeyVal==2)
			{
				txtfile=malloc(sizeof(FIL));
				if(f_stat( "0:easear.txt",&fno)==0)//如果已经存
				{
					char *buff=malloc(fno.fsize);//分配一个文件大小的内存空间，注意文件大小及分配结果
					f_open(txtfile, "0:easear.txt", FA_OPEN_EXISTING | FA_READ);
					if(f_read(txtfile, buff,fno.fsize, &bw)==0)
					{
						LCD_ShowString(0,150,200,16,12,(u8*)"easear.txt  read completed");
						LCD_ShowString(0,180,200,16,12,(u8*)"The content in the file is:");
						POINT_COLOR=BLUE;
						LCD_ShowString(0,195,200,16,12,(u8*)buff);
						POINT_COLOR=BLACK;
					}
					else
					{
					    LCD_ShowString(0,150,200,16,12,(u8*)"easear.txt  read err      ");
					}
					
					free(buff);
					
				}
				else
				{
					LCD_ShowString(0,150,200,16,12,(u8*)"easear.txt  does not exist");
				
				}
				free(txtfile);
			
			}

			KeySta=0;
		}
		
		delay_ms(50);
	}
}

