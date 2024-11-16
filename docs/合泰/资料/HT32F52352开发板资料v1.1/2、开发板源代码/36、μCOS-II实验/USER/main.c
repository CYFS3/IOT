#include "ht32.h"
#include "delay.h"
#include "led.h"







#include "includes.h" 


/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
#define START_TASK_PRIO      			20 //开始任务的优先级设置为最低
#define START_STK_SIZE  				64//设置任务堆栈大小
OS_STK START_TASK_STK[START_STK_SIZE];//任务堆栈	
void start_task(void *pdata);	//任务函数
 			  


//主任务
#define MAIN_TASK_PRIO       			7 //设置任务优先级
#define MAIN_STK_SIZE  		    		64//设置任务堆栈大小
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];//任务堆栈
void t_led1(void *pdata);            //外部定义              //任务函数
						 
//按键任务
#define KEY_TASK_PRIO       			8 //设置任务优先级
#define KEY_STK_SIZE  		    		64//设置任务堆栈大小
OS_STK KEY_TASK_STK[KEY_STK_SIZE];//任务堆栈
void t_led2(void *pdata);             //外部定义              //任务函数						 
						 	

						 
//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;  
	pdata = pdata; 		  
	OSStatInit();					//初始化统计任务.这里会延时1秒钟左右	
 	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)     				   
 	OSTaskCreate(t_led1,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 
    OSTaskCreate(t_led2,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	//按键任务	

 	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}



void t_led1(void *pdata)
{

	while(1)
	{
			led1(0); 
			delay_ms(50);
			led1(1); 
			delay_ms(500);	
	}


}

void t_led2(void *pdata)
{
	while(1)
	{
			led2(0); 
			delay_ms(500);
			led2(1); 
			delay_ms(500);	
	}
}

int main(void)
{

	delay_init();
	led_init();//初始化与LED连接的硬件接口
	OSInit();  	 			//初始化UCOSII	  
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();
	while (1);
}


