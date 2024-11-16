#include "ht32.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "gptm.h"


extern sPulseCaptureStructure CaptureCHx;



int main(void)
{
	
	delay_init();//延时函数初始化	  
	led_init();//初始化与LED连接的硬件接口
	uart_init(9600);
	gptm_cap_Init();
	
	printf("\r\n输入捕获实验\r\n");
	while(1)
	{    
		if (CaptureCHx.DataValid)
		{
		  printf("\r\n捕获到按键1时间 : %u mS\r\n", (int)(CaptureCHx.CapturePulse / (SystemCoreClock/1000ul)));
		  CaptureCHx.DataValid = FALSE;//重置捕获时间
		}
	}
}

