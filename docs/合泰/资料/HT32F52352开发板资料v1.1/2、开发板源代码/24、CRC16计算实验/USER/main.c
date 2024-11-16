#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "mctm.h"
#include "lcd.h"
#include "bftm.h"
#include "crc.h"





extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;







int main(void)
{

	u32 seed = 0xffff;
    u16 SW_sum, HW_sum;
    u8 crcs_dat[6] = {1,2,3,4,5,6};
	u8 crch_dat[6] = {1,2,3,4,5,6};
	char ch[40];
	delay_init();
	CRC_init();
	LCD_Init();
	
   /* CRC-16
   * width:   16-bit
   * poly:    0x8005
   * seed:    0x0000
   * XOROut:  0x0000
   * RefIn:   Yes
   * RefOut:  Yes
   */
	
	 SW_sum = crc16_fast((u16)seed, crcs_dat, sizeof(crcs_dat));//软件计算crc
	 HW_sum = CRC_16(seed, crch_dat, sizeof(crch_dat));         //硬件计算crc
  

	
  
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
  	POINT_COLOR=BLACK;		
	LCD_ShowString(0,0,240,20,16,(u8*) "Calculate the number CRC16: ");
	LCD_ShowString(0,20,240,20,16,(u8*)"0x01 0x02 0x03 0x04 0x05 0x06");
		
	
	POINT_COLOR=BLUE;  	

    sprintf(ch,"SoftWare:0x%04x",SW_sum);
	LCD_ShowString(0,60,240,20,16,(u8*) ch);
	sprintf(ch,"HardWare:0x%04x",HW_sum);
	LCD_ShowString(0,80,240,20,16,(u8*)ch);
	
	
	while(1)
	{		
		
	}
}

