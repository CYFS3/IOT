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
#include "fattester.h"
#include "ht32_usbd_core.h"
#include "ht32_usbd_class.h"
#include "ht32_usbd_descriptor.h"

//#include "_ht32_project_source.h"



extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

__ALIGN4 USBDCore_TypeDef gUSBCore;
USBD_Driver_TypeDef gUSBDriver;
u32 gIsLowPowerAllowed = TRUE;


void Suspend(u32 uPara)
{
  if (gIsLowPowerAllowed)
  {
    USBDCore_LowPower();
  }
  return;
}



void USB_Configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.USBD       = 1;
  CKCUClock.Bit.EXTI       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);


  #if (LIBCFG_CKCU_USB_PLL)
  USBPLL_Configuration();
  #endif

  #if (LIBCFG_PWRCU_VREG)
  USBVRG_Configuration();                               /* Voltage of USB setting                           */
  #endif

  gUSBCore.pDriver = (u32 *)&gUSBDriver;                /* Initiate memory pointer of USB driver            */
  gUSBCore.Power.CallBack_Suspend.func  = Suspend;      /* Install suspend call back function into USB core */
  //gUSBCore.Power.CallBack_Suspend.uPara = (u32)NULL;

  USBDDesc_Init(&gUSBCore.Device.Desc);                 /* Initiate memory pointer of descriptor            */
  USBDClass_Init(&gUSBCore.Class);                      /* Initiate USB Class layer                         */
  USBDCore_Init(&gUSBCore);                             /* Initiate USB Core layer                          */

  // Need turn on if the USB clock source is from HSI (PLL clock Source)
  #if 0
  CKCU_HSIAutoTrimClkConfig(CKCU_ATC_USB);
  CKCU_HSIAutoTrimCmd(ENABLE);
  #endif

  NVIC_EnableIRQ(USB_IRQn);                             /* Enable USB device interrupt      */
}

u16 nms=0;



int main(void)
{
	u8 res;
	
	delay_init();//延时函数初始化	  
	key_init();
	uart_init(115200);
	LCD_Init();
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
	SPI_Flash_Init();
	SD_Initialize();
	USB_Configuration();
	
	
	
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
	
	
	if(font_init())
	{
		update_font(20,110,16);
	}
	POINT_COLOR=BLACK;
	
	Show_Str(0,100,200,20,(u8*)"USB存储设备就绪！",16,1);
	USBD_DPpullupCmd(ENABLE);
	
	while(1)
	{		
		USBDCore_MainRoutine(&gUSBCore);	
	}
}


