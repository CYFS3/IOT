
#include "HT32.h"
#include "mctm.h"

uint32_t wCRR = 0, wPSCR = 0;
  
void mctm_pwm_Init(u32 fre)
{	
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;
  TM_OutputInitTypeDef TM_OutputInitStructure;

  CKCUClock.Bit.PB = 1;                                                                
  CKCUClock.Bit.MCTM0 = 1;
  CKCUClock.Bit.AFIO  = 1; 
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  AFIO_GPxConfig(GPIO_PB, GPIO_PIN_15, AFIO_FUN_MCTM_GPTM);
                                                                       
  wCRR = (SystemCoreClock / fre) - 1;
  while ((wCRR / (wPSCR + 1)) > 0xFFFF)
  {
    wPSCR++;
  }
  wCRR = wCRR / (wPSCR + 1);

                                                                            
  TM_TimeBaseInitStructure.CounterReload = wCRR;
  TM_TimeBaseInitStructure.Prescaler = wPSCR;
  TM_TimeBaseInitStructure.RepetitionCounter = 0;
  TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
  TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
  TM_TimeBaseInit(HT_MCTM0, &TM_TimeBaseInitStructure);
  

 
  TM_OutputInitStructure.Channel = TM_CH_0;
  TM_OutputInitStructure.OutputMode = TM_OM_PWM1;
  TM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
  TM_OutputInitStructure.ControlN = TM_CHCTL_DISABLE;//TM_CHCTL_ENABLE;
  TM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
  TM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
  TM_OutputInitStructure.IdleState = MCTM_OIS_HIGH;
  TM_OutputInitStructure.IdleStateN = MCTM_OIS_LOW;
  TM_OutputInitStructure.Compare = ((wCRR + 1) * (50)) / 100;
  TM_OutputInit(HT_MCTM0, &TM_OutputInitStructure);                                 
                                    
  TM_CHCCRPreloadConfig(HT_MCTM0,TM_CH_0,ENABLE);
  TM_CRRPreloadCmd(HT_MCTM0,ENABLE);
  
  MCTM_CHMOECmd(HT_MCTM0, ENABLE);                                     
  TM_Cmd(HT_MCTM0, ENABLE); 
  
}
                                  

//mtch0  pb15 ÓÃ×÷audio Êä³ö
//0-2400  @ 20khz
void mctm_pwm(u16 per)
{
	TM_SetCaptureCompare(HT_MCTM0, TM_CH_0, per);
}





