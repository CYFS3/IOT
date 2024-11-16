
#include "HT32.h"
#include "led.h"


void bftm_Init(u32 nus) 
{	
  
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.BFTM0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
  

  NVIC_EnableIRQ(BFTM0_IRQn);
                                                 
  BFTM_SetCompare(HT_BFTM0, SystemCoreClock/1000000*nus);
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
  BFTM_EnaCmd(HT_BFTM0, ENABLE);

}



//bftmÖÐ¶Ïº¯Êý
void BFTM0_IRQHandler(void)
{
	static u16 cnt_1s=0;
	cnt_1s++;
	if(cnt_1s<250)
	{
		led1(1);
	}
	else if(cnt_1s<500)
	{
		led1(0);
	}
	else
	{
		cnt_1s=0;
	}

	BFTM_ClearFlag(HT_BFTM0);
}

