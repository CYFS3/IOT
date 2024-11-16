#include "ht32.h"



void led_init(void)
{
	
	//ʹ��PC�˿ڵ�ʱ��
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PC         = 1;
	CKCUClock.Bit.AFIO       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	//���ö˿ڹ���ΪGPIO
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_15, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_14, AFIO_FUN_GPIO);

	//����IO��Ϊ���ģʽ                                                     
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_15, GPIO_DIR_OUT);
	GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_14, GPIO_DIR_OUT);

	//Ĭ�����1
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, SET);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, SET);

}
//����LED1���  SET:1  RESET:0
void led1(u8 a)
{
	if(a)
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, SET);
	else
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, RESET);
}

//����LED1���  SET:1  RESET:0
void led2(u8 a)
{
	if(a)
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, SET);
	else
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, RESET);
}


