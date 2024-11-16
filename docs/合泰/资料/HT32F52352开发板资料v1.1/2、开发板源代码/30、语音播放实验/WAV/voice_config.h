
#ifndef __VOICE_CONFIG_H
#define __VOICE_CONFIG_H

#include "ht32.h"



//**************************************************************************************//
#define VOICE_TM_IRQHandler  MCTM0_IRQHandler

#define Timer_Flag                    3
#define Timer_ADDR                    0x4002C000
#define Timer_ADDR1                   0x4002C000
#define Timer_PWM1_GPIO_Type          1
#define Timer_PWM1_PIN_Type           GPIO_PIN_15
#define Timer_PWM1_AF_Type            AFIO_MODE_4
#define Timer_PWM1_CHx_Type           TM_CH_0
#define Timer_PWM1_Mode_Type          TM_OM_PWM1
#define Timer_PWM2_GPIO_Type          1 
#define Timer_PWM2_PIN_Type           GPIO_PIN_0
#define Timer_PWM2_AF_Type            AFIO_MODE_DEFAULT //no use
#define Timer_PWM2_CHx_Type           TM_CH_1
#define Timer_PWM2_Mode_Type          TM_OM_PWM1
#define Timer_IRQ_Type                MCTM0_IRQn


/*Note:  don't support sctm
Timer_Flag:

            0   PWM
						1   GPTM
						2   SCTM
						3   MCTM

Timer_ADDR:     
            0x40031000   PWM0  
						0x40071000   PWM1
						0x4006E000   GPTM0 
						0x4006F000   GPTM1
					  0x40034000   SCTM0
						0x40074000   SCTM1
						0x40035000   SCTM2
						0x40075000   SCTM3
            0x4002C000   MCTM0   
								 
Timer_PWMx_GPIO_Type:				
            0          GPIO_PA
						1          GPIO_PB
					  2          GPIO_PC
					  3          GPIO_PD
						......
						
Timer_PWMx_PIN_Type:  
           
					 GPIO_PIN_0 ~ GPIO_PIN_15

Timer_PWMx_AF_Type:

					 AFIO_MODE_DEFAULT ~ AFIO_MODE_15
           0 ~ 15
	
Timer_PWMx_CHx_Type:
        
				   TM_CH_0 ~ TM_CH_3
				   0 ~ 3 
	
Timer_PWMx_Mode_Type:

					TM_OM_MATCH_NOCHANGE  = 0x0000, !< TM channel output no change on match                                 
					TM_OM_MATCH_INACTIVE  = 0x0001, !< TM channel output inactive level on match                            
					TM_OM_MATCH_ACTIVE    = 0x0002, !< TM channel output active level on match                              
					TM_OM_MATCH_TOGGLE    = 0x0003, !< TM channel output toggle on match                                    
					TM_OM_FORCED_INACTIVE = 0x0004, !< TM channel output forced inactive level                              
					TM_OM_FORCED_ACTIVE   = 0x0005, !< TM channel output forced active level                                
					TM_OM_PWM1            = 0x0006, !< TM channel pwm1 output mode                                          
					TM_OM_PWM2            = 0x0007, !< TM channel pwm2 output mode                                          
					TM_OM_ASYMMETRIC_PWM1 = 0x0106, !< TM channel asymmetric pwm1 output mode                               
					TM_OM_ASYMMETRIC_PWM2 = 0x0107  !< TM channel asymmetric pwm2 output mode                               
    
Timer_IRQ_Type:		
     
		      IRQn_Type                           																									*/                
//*********************************************************************************************//

typedef struct
{
	  
	TM_OM_Enum  mode;
	
//AF Configuration
  u8  gpiox;          // 0 GPIO_PA
                       // 1 GPIO_PB
	                     // 2 GPIO_PC
	                     // 3 GPIO_PD
	                     // 4  GPIO_PE
	                     // 5  GPIO_PF
	
	AFIO_MODE_Enum AFx;  // 0 AFIO_MODE_DEFAULT   
	                     // 1 AFIO_MODE_1
	                     // 2 AFIO_MODE_2
	                     //   ......
	                     // 15 AFIO_MODE_15
	
	u32 pinx;           //GPIO_PIN_0    0x0001 
											//GPIO_PIN_1    0x0002 
											//GPIO_PIN_2    0x0004
											//GPIO_PIN_3    0x0008 	
											//GPIO_PIN_4    0x0010
											//   ......
											//GPIO_PIN_ALL  0xFFFF
	             
	
	TM_CH_Enum chx;     //TM_CH_0   0
	                    //TM_CH_1   1
											//TM_CH_2   2
											//TM_CH_3   3
	 
}PWM_TypeDef;


typedef struct
{
	
	u32 flag;
//Timer Configuration
	u32 tm_base_addr;   // tiemr base address
	u32 tm_base_addr1;
	
	PWM_TypeDef  pwm1;
	PWM_TypeDef  pwm2;
	IRQn_Type   irq;
			
}VOICE_INIT_TypeDef;

extern VOICE_INIT_TypeDef voice;

void PLAY_VOICE(u32 audLen, const short *audData, u8 VOLUME);
void VOICE_Pause(void);
void VOICE_Continue(void);
u8 VOICE_PlayVoiceFinish(void);
void VOICE_Init(void);

void voice_timer_handle(void);
void Voice_VarInit(void);
#endif

