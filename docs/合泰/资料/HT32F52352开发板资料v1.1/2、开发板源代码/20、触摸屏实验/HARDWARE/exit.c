#include "ht32.h"

u8 button1_exit=0;
u8 button2_exit=0;

void exit_init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct = {0};
	
	//ʹ��PC�˿ڵ�ʱ��
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PA         = 1;
	CKCUClock.Bit.PC         = 1;
	CKCUClock.Bit.AFIO       = 1;
	CKCUClock.Bit.EXTI       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	//���ö˿ڹ���ΪGPIO
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_8, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_13, AFIO_FUN_GPIO);

	//����IO��Ϊ����ģʽ                                                     
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_8, GPIO_DIR_IN);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_13, GPIO_DIR_IN);

	//ʹ������
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_8,ENABLE);
	GPIO_InputConfig(HT_GPIOC, GPIO_PIN_13,ENABLE);

	//������������
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_8,GPIO_PR_UP);
	GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_13,GPIO_PR_UP);
	

    //�ж�Դ����
    AFIO_EXTISourceConfig(AFIO_EXTI_CH_8,AFIO_ESS_PA);
	AFIO_EXTISourceConfig(AFIO_EXTI_CH_13,AFIO_ESS_PC);

    //�жϷ�ʽ����
    EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_8;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;//����ȥ��
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;//�½��ش���
    EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_13;
    EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;//����ȥ��
    EXTI_InitStruct.EXTI_DebounceCnt = 0;
    EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;//�½��ش���
    EXTI_Init(&EXTI_InitStruct);

    //ʹ���ж�Դ
    EXTI_IntConfig(EXTI_CHANNEL_8, ENABLE);
	EXTI_IntConfig(EXTI_CHANNEL_13, ENABLE);
	
	//ʹ���ж�
	NVIC_EnableIRQ(EXTI4_15_IRQn);//exit 4-15����һ���ж�����
	
}


//exit 4-15�жϺ���
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


