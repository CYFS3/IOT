#include "ht32.h"
#include "usart.h"	  


/*使用microLib的方法*/
 
int fputc(int ch, FILE *f)
{
	USART_SendData(HT_USART0, (uint8_t) ch);
	while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET) {}	
    return ch;
}
int GetKey (void)  { 
    while (!(HT_USART0->SR & USART_FLAG_RXDNE));
    return ((int)(HT_USART0->DR & 0x1FF));
}

 
#if EN_USART0_RX   //如果使能了接收
//串口0中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

//初始化IO 串口0
//bound:波特率
void uart_init(u32 bound)
{
  USART_InitTypeDef USART_InitStructure;
                                                               
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.USART0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Config AFIO mode as COM1_Rx and COM1_Tx function.                                                      */
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_FUN_USART_UART);

  
 
  /* COM1 configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - None parity bit
  */
  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
  USART_Init(HT_USART0, &USART_InitStructure);


  /* Enable COM1_PORT                                                                                       */
  USART_TxCmd(HT_USART0, ENABLE);
  
	
	
#if EN_USART0_RX		  //如果使能了接收             
    USART_IntConfig(HT_USART0, USART_INT_RXDR, ENABLE);//配置接收中断
	USART_RxCmd(HT_USART0, ENABLE);
    NVIC_EnableIRQ(USART0_IRQn);
#endif

}


/*********************************************************************************************************//**
 * @brief   This function handles USART interrupt.
 * @retval  None
 ************************************************************************************************************/
void USART0_IRQHandler(void)//串口0中断服务程序
{

	u8 Res;

	if (USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(HT_USART0);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
     } 

} 
#endif	

