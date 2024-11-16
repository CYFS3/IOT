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
#include "bmp.h"
#include "bftm.h"
#include "fattester.h"



extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;


u16 nms=0;

int main(void)
{
	u8 res;
	FILINFO fno;
	delay_init();//��ʱ������ʼ��	  
	key_init();
	uart_init(9600);
	LCD_Init();
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
	bftm_Init(1000);
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
		update_font(20,110,16);
	}
	POINT_COLOR=BLACK;
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

	
	Show_Str(0,0,240,16,(u8*)"����ͼƬ��FLASH...",16,1);
	if(f_stat( "1:ɨ���ά����꿴��.bin",&fno))//�ļ�����������
	mf_copy((u8*)"0:/BMP/ɨ���ά����꿴��.bin",(u8*)"1:ɨ���ά����꿴��.bin",FA_CREATE_ALWAYS);
	if(f_stat( "1:EASEAR.bin",&fno))//�ļ�����������
	mf_copy((u8*)"0:/BMP/EASEAR.bin",(u8*)"1:EASEAR.bin",FA_CREATE_ALWAYS);
	if(f_stat( "1:����1.bin",&fno))//�ļ�����������
	mf_copy((u8*)"0:/BMP/����1.bin",(u8*)"1:����1.bin",FA_CREATE_ALWAYS);
	if(f_stat( "1:����2.bin",&fno))//�ļ�����������
	mf_copy((u8*)"0:/BMP/����2.bin",(u8*)"1:����2.bin",FA_CREATE_ALWAYS);
	if(f_stat( "1:����3.bin",&fno))//�ļ�����������
	mf_copy((u8*)"0:/BMP/����3.bin",(u8*)"1:����3.bin",FA_CREATE_ALWAYS);
	if(f_stat( "1:����4.bin",&fno))//�ļ�����������
	mf_copy((u8*)"0:/BMP/����4.bin",(u8*)"1:����4.bin",FA_CREATE_ALWAYS);
	
	Show_Str(0,0,240,16,(u8*)"�������          ",16,1);
	while(1)
	{		
		

		nms=0;
		dispbinpic((u8*)"1:ɨ���ά����꿴��.bin",0,0,240,320);
		LCD_ShowxNum(0,20,nms,4,16,1);
		Show_Str(0,0,240,16,(u8*)"ɨ���ά����꿴��.bin",16,1);
		delay_ms(5000);
		
		LCD_Clear(WHITE);
		nms=0;
		dispbinpic((u8*)"1:EASEAR.bin",0,100,240,97);
		LCD_ShowxNum(0,20,nms,4,16,1);
		Show_Str(0,0,240,16,(u8*)"EASEAR.bin",16,1);
		delay_ms(1000);
		
		nms=0;
		dispbinpic((u8*)"1:����1.bin",0,0,240,320);
		LCD_ShowxNum(0,20,nms,4,16,1);
		Show_Str(0,0,240,16,(u8*)"����1.bin",16,1);
		delay_ms(1000);

		nms=0;
		dispbinpic((u8*)"1:����2.bin",0,0,240,320);
		LCD_ShowxNum(0,20,nms,4,16,1);
		Show_Str(0,0,240,16,(u8*)"����2.bin",16,1);
		delay_ms(1000);
		
		nms=0;
		dispbinpic((u8*)"1:����3.bin",0,0,240,320);
		LCD_ShowxNum(0,20,nms,4,16,1);
		Show_Str(0,0,240,16,(u8*)"����3.bin",16,1);
		delay_ms(1000);
		
		nms=0;
		dispbinpic((u8*)"1:����4.bin",0,0,240,320);
		LCD_ShowxNum(0,20,nms,4,16,1);
		Show_Str(0,0,240,16,(u8*)"����4.bin",16,1);
		delay_ms(1000);		
	
	}
}


//bftm�жϺ���
void BFTM0_IRQHandler(void)
{

	nms++;
	BFTM_ClearFlag(HT_BFTM0);
}

