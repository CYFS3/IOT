#include "ht32.h"
#include "delay.h"
#include "led.h"







#include "includes.h" 


/////////////////////////UCOSII��������///////////////////////////////////
//START ����
#define START_TASK_PRIO      			20 //��ʼ��������ȼ�����Ϊ���
#define START_STK_SIZE  				64//���������ջ��С
OS_STK START_TASK_STK[START_STK_SIZE];//�����ջ	
void start_task(void *pdata);	//������
 			  


//������
#define MAIN_TASK_PRIO       			7 //�����������ȼ�
#define MAIN_STK_SIZE  		    		64//���������ջ��С
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];//�����ջ
void t_led1(void *pdata);            //�ⲿ����              //������
						 
//��������
#define KEY_TASK_PRIO       			8 //�����������ȼ�
#define KEY_STK_SIZE  		    		64//���������ջ��С
OS_STK KEY_TASK_STK[KEY_STK_SIZE];//�����ջ
void t_led2(void *pdata);             //�ⲿ����              //������						 
						 	

						 
//��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;  
	pdata = pdata; 		  
	OSStatInit();					//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)     				   
 	OSTaskCreate(t_led1,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 
    OSTaskCreate(t_led2,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	//��������	

 	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
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
	led_init();//��ʼ����LED���ӵ�Ӳ���ӿ�
	OSInit();  	 			//��ʼ��UCOSII	  
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();
	while (1);
}


