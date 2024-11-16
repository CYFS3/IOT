
#include "HT32.h"
#include "bftm.h"




#define User_code 0xff00 

#define IR_sample 125 

#define IR_SYNC_MAX                (15000/IR_sample)
#define IR_SYNC_MIN                (9700 /IR_sample)
#define IR_SYNC_DIVIDE             (12375 /IR_sample)
#define IR_DATA_MAX                (3000 /IR_sample)
#define IR_DATA_MIN                (600  /IR_sample)
#define IR_DATA_DIVIDE             (1687 /IR_sample)
#define IR_BIT_NUMBER              32      

volatile u8 IR_SampleCnt;
volatile u8 IR_BitCnt;
volatile u8 IR_UserH;
volatile u8 IR_UserL;
volatile u8 IR_data;
volatile u8 IR_DataShit;
volatile u8 IR_code;           
volatile u8 Ir_Pin_temp;
volatile u8 IR_Sync;                    
volatile u8 IrUserErr;                    
volatile u8 IR_OK;                        
volatile u8 F0;

void ir_Init(void) 
{	
	//使能PC端口的时钟
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PC         = 1;
	CKCUClock.Bit.AFIO       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	//配置端口功能为GPIO
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_0, AFIO_FUN_GPIO);

	//配置IO口为输入模式                                                     
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_0, GPIO_DIR_IN);

	//使能输入
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_0,ENABLE);
	
	bftm_Init(125);//200us

  
}

void BFTM0_IRQHandler(void)
{
	u8 SampleTime;
	IR_SampleCnt++;
	F0=Ir_Pin_temp;        
	Ir_Pin_temp=GPIO_ReadInBit(HT_GPIOC,GPIO_PIN_0);
	if(F0&&(!Ir_Pin_temp))
	{
			
		SampleTime=IR_SampleCnt;
		IR_SampleCnt=0;
		if(SampleTime>IR_SYNC_MAX)
		{
			IR_Sync=0;
		}
		else if(SampleTime>=IR_SYNC_MIN)
		{
			
			if(SampleTime>=IR_SYNC_DIVIDE)
			{
				
				IR_Sync=1;
				IR_BitCnt=IR_BIT_NUMBER;
			}
		}
		else if(IR_Sync) 
		{
			if((SampleTime<IR_DATA_MIN)|(SampleTime>IR_DATA_MAX))IR_Sync=0;
			else
			{
				IR_DataShit>>=1; 
				if(SampleTime>=IR_DATA_DIVIDE)IR_DataShit|=0x80;
				
				if(--IR_BitCnt==0)                            
				{
					IR_Sync=0; 
					
					if((IR_UserH==(User_code/256))&&IR_UserL==(User_code%256))
					{
						IrUserErr=0;
						IR_code=IR_data;
						IR_OK=1;
					}
					else IrUserErr=1; 
				}
				else if((IR_BitCnt&7)==0)
				{
					IR_UserL=IR_UserH;
					IR_UserH=IR_data;
					IR_data=IR_DataShit;
				}
			}
		}  
	}
	BFTM_ClearFlag(HT_BFTM0);
}


