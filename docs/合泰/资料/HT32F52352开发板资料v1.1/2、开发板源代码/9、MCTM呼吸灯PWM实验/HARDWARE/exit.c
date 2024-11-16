#include "ht32.h"

u8 button1_exit=0;
u8 button2_exit=0;

void exit_init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct = {0};
	
	//使能PC端口的时钟
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PA         = 1;
	CKCUClock.Bit.PC         = 1;
	CKCUClock.Bit.AFIO       = 1;
	CKCUClock.Bit.EXTI       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	//配置端口功能为GPIO
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_8, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_13, AFIO_FUN_GPIO);

	//配置IO口为输入模式                                                     
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_8, GPIO_DIR_IN);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_13, GPIO_DIR_IN);

	//使能输入
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_8,ENABLE);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_13,ENABLE);

	//配置上拉电阻
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_8,GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_13,GPIO_PR_UP);
	

    //中断源配置
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_8,AFIO_ESS_PA);
	AFIO_EXTISourceConfig(AFIO_EXTI_CH_13,AFIO_ESS_PC);

    //中断方式配置
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_8;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;//禁用去抖
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;//下降沿触发
    EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_13;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;//禁用去抖
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;//下降沿触发
    EXTI_Init(&EXTI_InitStruct);

    //使能中断源
    EXTI_IntConfig(EXTI_CHANNEL_8, ENABLE);
	EXTI_IntConfig(EXTI_CHANNEL_13, ENABLE);
	
	//使能中断
	NVIC_EnableIRQ(EXTI4_15_IRQn);//exit 4-15共用一个中断向量
	
}


//exit 4-15中断函数
void EXTI4_15_IRQHandler(void)
{
  if(EXTI_GetEdgeFlag(EXTI_CHANNEL_8))
  {
    EXTI_ClearEdgeFlag(EXTI_CHANNEL_8);
    button1_exit=1;
  }
  if(EXTI_GetEdgeFlag(EXTI_CHANNEL_13))
  {
    EXTI_ClearEdgeFlag(EXTI_CHANNEL_13);
    button2_exit=1;
  }
}


