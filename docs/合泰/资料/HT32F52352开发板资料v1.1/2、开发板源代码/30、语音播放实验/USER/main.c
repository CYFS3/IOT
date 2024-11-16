/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "voice_config.h"


extern const short __voice1_table[8800];



int main(void)
{

	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO         = 1;
	CKCUClock.Bit.MCTM0  = 1;
	CKCUClock.Bit.PB  = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	Voice_VarInit();
	
	VOICE_Init();  
	
  while (1)
  {
		PLAY_VOICE(sizeof(__voice1_table), __voice1_table, 0);
		while(VOICE_PlayVoiceFinish()==0);
		
  }
}


void Voice_VarInit(void)
{
	voice.flag = Timer_Flag;
	voice.tm_base_addr = Timer_ADDR  ; 
	voice.tm_base_addr1 = Timer_ADDR1  ; 
	voice.pwm1.gpiox = Timer_PWM1_GPIO_Type;              
	voice.pwm1.pinx = Timer_PWM1_PIN_Type;
	voice.pwm1.AFx = Timer_PWM1_AF_Type;      
	voice.pwm1.chx = Timer_PWM1_CHx_Type;         
	voice.pwm1.mode = Timer_PWM1_Mode_Type;     
	voice.pwm2.gpiox = Timer_PWM2_GPIO_Type;             
	voice.pwm2.pinx = Timer_PWM2_PIN_Type;
	voice.pwm2.AFx = Timer_PWM2_AF_Type;      
	voice.pwm2.chx = Timer_PWM2_CHx_Type;        
	voice.pwm2.mode = Timer_PWM2_Mode_Type;     
	voice.irq = Timer_IRQ_Type;   
}



void VOICE_TM_IRQHandler(void)
{
		voice_timer_handle();
}
