#include "delay.h"
#include "ht32.h"

static u8  fac_us=0;//us延时倍乘数

//初始化延迟函数
void delay_init()	 
{
	SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);	//选择外部时钟  HCLK/8=6M
	fac_us=SystemCoreClock/8000000;	//为系统时钟的1/8  
}								    

//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}


//延时nms
void delay_ms(u16 nms)
{	 		 
		while(nms--)
	    delay_us(1000);
} 

