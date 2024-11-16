#include "ht32.h"
#include "usart.h"	  


/*ʹ��microLib�ķ���*/
 
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

 
#if EN_USART0_RX   //���ʹ���˽���
//����0�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

//��ʼ��IO ����0
//bound:������
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
  
	
	
#if EN_USART0_RX		  //���ʹ���˽���             
    USART_IntConfig(HT_USART0, USART_INT_RXDR, ENABLE);//���ý����ж�
	USART_RxCmd(HT_USART0, ENABLE);
    NVIC_EnableIRQ(USART0_IRQn);
#endif

}


/*********************************************************************************************************//**
 * @brief   This function handles USART interrupt.
 * @retval  None
 ************************************************************************************************************/
void USART0_IRQHandler(void)//����0�жϷ������
{

	u8 Res;

	if (USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(HT_USART0);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
     } 

} 
#endif	

