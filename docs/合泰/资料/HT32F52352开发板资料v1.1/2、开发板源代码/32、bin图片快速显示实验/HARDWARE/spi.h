#ifndef __SPI_H
#define __SPI_H
#include "ht32.h"

 				  	    													  
void SPI0_Init(void);			 //��ʼ��SPI��
void SPI0_SetSpeed(u32 Guard_Time); //����SPI�ٶ�   
u8 SPI0_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�


void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u32 Guard_Time); //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(vu32 TxData);//SPI���߶�дһ���ֽ�

#endif

