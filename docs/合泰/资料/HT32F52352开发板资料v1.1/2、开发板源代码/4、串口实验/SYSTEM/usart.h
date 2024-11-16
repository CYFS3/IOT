#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "ht32.h" 

#define RECFINISH    			0x8000  	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART0_RX 			1		//ʹ�ܣ�1��/��ֹ��0������0����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
void uart_init(u32 bound);


#endif


