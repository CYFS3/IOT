
#include "HT32.h"

void wdt_Init(u16 prer,u16 reload) 
{	
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.WDT = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  WDT_DeInit();    
  WDT_SetPrescaler(prer);                                                       
  WDT_SetReloadValue(reload);
  WDT_ResetCmd(ENABLE);    //���Ź����ʱʹ�ܸ�λ����
  WDT_Restart();                    // Reload Counter as WDTV Value
  WDT_Cmd(ENABLE);                  // Enable WDT
  
  
  
  
  
}
//ι�������Ź�
void WDT_Feed(void)
{   
 	WDT_Restart(); //reload										   
}


