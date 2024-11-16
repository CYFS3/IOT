#include "ht32.h"
#include "key.h"
u8 KeyStatus=0;
u16 KeyCnt=0;

u8 KeyVal=0;
u8 KeySta=0;

void key_init(void)
{
	
	//ʹ��PC�˿ڵ�ʱ��
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PA         = 1;
	CKCUClock.Bit.PC         = 1;
	CKCUClock.Bit.AFIO       = 1;
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
	

}

//��ȡ������ӦIO״̬
unsigned char getkey()
{
	u8 temp=0;
	if(GPIO_ReadInBit(HT_GPIOA,GPIO_PIN_8)==RESET)
	{
		temp|=0x02;
	}
	if(GPIO_ReadInBit(HT_GPIOC,GPIO_PIN_13)==RESET)
	{
		temp|=0x01;
	}
	return temp;

}

//ɨ�谴��
void keyscan()
{
	u8 temp;
	
	temp=getkey();
	
	switch(KeyStatus)
	{
		case WaitStatus:
			  if(temp!=0x00)
			  {
						KeyStatus=PressStatus;
			  }
			  break;

	   case PressStatus: 
			if(temp!=0x00)
			{
				KeyVal=temp;
				KeySta=KEYPRESS;
				KeyStatus=ReleaseStatus;
			}
			else
			{
				KeyStatus=WaitStatus;
			}
			break;

		case ReleaseStatus: 
			if(temp!=0x00)
			{
				if(++KeyCnt>=KeyTime)//������ʱ��Ϊkeyscan()��������*KeyTime
				{
					KeySta=KEYLONG;
					KeyStatus=IDEStatus;
					KeyCnt=0;
				}
			}
			else
			{
					KeyCnt=0;
					KeyStatus=WaitStatus;
					KeySta=KEYRELEASE;
			}
			break;
		case IDEStatus:
			if(temp==0x00)
			{
				KeyStatus=WaitStatus;
			}
		break;
	}
}

