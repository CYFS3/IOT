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
    u16 SW_sum, HW_sum,PDMA_sum;
    u8 crcs_dat[6] = {1,2,3,4,5,6};
	u8 crch_dat[6] = {1,2,3,4,5,6};
	u8 crcp_dat[6] = {1,2,3,4,5,6};
	
	char ch[40];
	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	PDMACH_InitTypeDef PDMACH_InitStructure;
	CRC_InitTypeDef CRC_InitStructure;
	CKCUClock.Bit.PDMA       = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	delay_init();
	CRC_init();
	LCD_Init();
	
  /* CRC-CCITT
   * width:   16-bit
   * poly:    0x1021
   * seed:    0xFFFF
   * XOROut:  0x0000
   * RefIn:   No
   * RefOut:  No
   */
	
	 SW_sum = crc_ccitt_fast((u16)seed, crcs_dat, sizeof(crcs_dat));//软件计算crc
	 HW_sum = CRC_CCITT(seed, crch_dat, sizeof(crch_dat));         //硬件计算crc
  
	
	  CRC_InitStructure.Mode = CRC_CCITT_POLY;
	  CRC_InitStructure.uSeed = seed;
	  CRC_Init(HT_CRC, &CRC_InitStructure);

	  PDMACH_InitStructure.PDMACH_SrcAddr   = (u32)crcp_dat;//源地址
	  PDMACH_InitStructure.PDMACH_DstAddr   = (u32)&HT_CRC->DR;//目标地址
	  PDMACH_InitStructure.PDMACH_AdrMod    = (SRC_ADR_LIN_INC | DST_ADR_FIX);
	  PDMACH_InitStructure.PDMACH_Priority  = L_PRIO;
	  PDMACH_InitStructure.PDMACH_BlkCnt    = sizeof(crcp_dat)/3;
	  PDMACH_InitStructure.PDMACH_BlkLen    = 3;
	  PDMACH_InitStructure.PDMACH_DataSize  = WIDTH_8BIT;
	  PDMA_Config(PDMA_CH0, &PDMACH_InitStructure);
	  PDMA_IntConfig(PDMA_CH0, (PDMA_INT_GE | PDMA_INT_TC), ENABLE);
	  PDMA_EnaCmd(PDMA_CH0, ENABLE);
	  PDMA_SwTrigCmd(PDMA_CH0, ENABLE);

	  while (PDMA_GetFlagStatus(PDMA_CH0, PDMA_FLAG_TC) == RESET);
	  PDMA_ClearFlag(PDMA_CH0, PDMA_FLAG_TC);
	  PDMA_sum = CRC_Process(HT_CRC, NULL, 0);



	
  
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
	
  	POINT_COLOR=BLACK;		
	LCD_ShowString(0,0,240,20,16,(u8*) "Calculate the number CRC-CCITT");
	LCD_ShowString(0,20,240,20,16,(u8*)"0x01 0x02 0x03 0x04 0x05 0x06 ");
		
	
	POINT_COLOR=BLUE;  	

    sprintf(ch,"SoftWare:0x%04x",SW_sum);
	LCD_ShowString(0,60,240,20,16,(u8*) ch);
	sprintf(ch,"HardWare:0x%04x",HW_sum);
	LCD_ShowString(0,80,240,20,16,(u8*)ch);
	sprintf(ch,"Move by PDMA:0x%04x",PDMA_sum);
	LCD_ShowString(0,100,240,20,16,(u8*)ch);
	
	while(1)
	{		
		
	}
}

