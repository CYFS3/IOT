#ifndef __SPI_H
#define __SPI_H
#include "ht32.h"

 				  	    													  
void SPI0_Init(void);			 //初始化SPI口
void SPI0_SetSpeed(u32 Guard_Time); //设置SPI速度   
u8 SPI0_ReadWriteByte(u8 TxData);//SPI总线读写一个字节


void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u32 Guard_Time); //设置SPI速度   
u8 SPI1_ReadWriteByte(vu32 TxData);//SPI总线读写一个字节

#endif

