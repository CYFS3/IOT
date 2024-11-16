#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "mpu6050.h"
#include "key.h"
#include "usart.h"
#include "mctm.h"
#include "lcd.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 



extern u8 f1s;

extern u8 KeyVal;
extern u8 KeySta;

int main(void)
{
	u8 t=0;
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;					//�¶�	
	
	char ch[40];
	delay_init();//��ʱ������ʼ��	 
	uart_init(9600);
	LCD_Init();
	
	
	MPU_Init();
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");

	LCD_ShowString(0,0,200,16,16,(u8*)"MPU6050 init...");
	
	while(mpu_dmp_init())
 	{
		LCD_ShowString(0,0,200,16,16,(u8*)"MPU6050 Error");
		delay_ms(200);
		LCD_ShowString(0,0,200,16,16,(u8*)"                ");
 		delay_ms(200);
	}  

	LCD_ShowString(0,0,200,16,16,(u8*)"MPU6050 OK      ");
	while(1)
	{		
		printf("hello");
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			if(++t>10)
			{
				t=0;
				LCD_ShowString(0,20,200,16,16,(u8*)"Acceleration:");
				sprintf(ch,"X:%d Y:%d Z:%d          ",aacx,aacy,aacz);
				POINT_COLOR=BLUE;
				LCD_ShowString(0,40,200,16,16,(u8*)ch);
				POINT_COLOR=BLACK;
				
				
				LCD_ShowString(0,80,200,16,16,(u8*)"Gyroscope:");
				sprintf(ch,"X:%d Y:%d Z:%d          ",gyrox,gyroy,gyroz);
				POINT_COLOR=BLUE;
				LCD_ShowString(0,100,200,16,16,(u8*)ch);
				POINT_COLOR=BLACK;
				
				LCD_ShowString(0,140,200,16,16,(u8*)"Euler angles:");
				sprintf(ch,"P:%4.2f R:%4.2f Y:%4.2f          ",pitch,roll,yaw);
				POINT_COLOR=BLUE;
				LCD_ShowString(0,160,200,16,16,(u8*)ch);
				POINT_COLOR=BLACK;
				
				LCD_ShowString(0,200,200,16,16,(u8*)"Temperature:");
				sprintf(ch,"%.1fC    ",(float)temp/100.0);
				POINT_COLOR=BLUE;
				LCD_ShowString(0,220,200,16,16,(u8*)ch);
				POINT_COLOR=BLACK;
			}
		}
		
	}
}

