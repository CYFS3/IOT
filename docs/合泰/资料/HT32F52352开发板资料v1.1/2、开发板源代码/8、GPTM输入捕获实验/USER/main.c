#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "gptm.h"


extern sPulseCaptureStructure CaptureCHx;



int main(void)
{
	
	delay_init();//��ʱ������ʼ��	  
	led_init();//��ʼ����LED���ӵ�Ӳ���ӿ�
	uart_init(9600);
	gptm_cap_Init();
	
	printf("\r\n���벶��ʵ��\r\n");
	while(1)
	{    
		if (CaptureCHx.DataValid)
		{
		  printf("\r\n���񵽰���1ʱ�� : %u mS\r\n", (int)(CaptureCHx.CapturePulse / (SystemCoreClock/1000ul)));
		  CaptureCHx.DataValid = FALSE;//���ò���ʱ��
		}
	}
}

