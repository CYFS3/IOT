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
	delay_init();//��ʱ������ʼ��	  
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
	
	
	res=f_mount(fs[0],"0:",1);//����SD�� 
	if(res==0)
	{
		exf_getfree((u8*)"0",&totalrom,&freerom);	//�õ�SD������������ʣ������ 
		LCD_ShowString(0,60,200,16,16,(u8*)"SD FATFS OK!");	 
		LCD_ShowString(0,80,200,16,16,(u8*)"SD Total Size:     MB");	 
		LCD_ShowString(0,100,200,16,16,(u8*)"SD  Free Size:     MB"); 	    
		LCD_ShowNum(112,80,totalrom>>10,5,16);				//��ʾSD�������� MB
		LCD_ShowNum(112,100,freerom>>10,5,16);					//��ʾSD��ʣ������ MB	
	}
	else
	{
		LCD_ShowString(0,60,200,16,16,(u8*)"SD CARD ERR!");	
		while(1);
	}
	
 	res=f_mount(fs[1],"1:",1);//����FLASH.	
	if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
	{
		LCD_ShowString(0,140,200,16,16,(u8*)"Flash Disk Formatting...");	    //��ʽ��FLASH
		res=f_mkfs("1:",1,4096);                                                //��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:EASEAR");	                            //����Flash���̵�����
			LCD_ShowString(0,140,200,16,16,(u8*)"Flash Disk Format Finish");	//��ʽ�����
		}else LCD_ShowString(0,140,200,16,16,(u8*)"Flash Disk Format Error ");	//��ʽ��ʧ��
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
				if(f_stat( "0:easear.txt",&fno)==0)//����Ѿ�������ɾ��
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
				if(f_stat( "0:easear.txt",&fno)==0)//����Ѿ���
				{
					char *buff=malloc(fno.fsize);//����һ���ļ���С���ڴ�ռ䣬ע���ļ���С��������
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

