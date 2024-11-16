#include "ht32.h"


void beep_init(void)
{
	
	//ʹ��PC�˿ڵ�ʱ��
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PA         = 1;
	CKCUClock.Bit.AFIO       = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);

	//���ö˿ڹ���ΪGPIO
	AFIO_GPxConfig(GPIO_PA, GPIO_PIN_10, AFIO_FUN_GPIO);

	//����IO��Ϊ���ģʽ                                                     
	GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10, GPIO_DIR_OUT);

	//Ĭ�����0
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_10, RESET);

}
//����LED1���  SET:1  RESET:0
void beep(u8 a)
{
	if (a)
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_10, SET);
	else
	GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_10, RESET);	
}
