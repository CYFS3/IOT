#include "ht32.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "bftm.h"
#include "iap.h"




vu8 rxdata[520];
vu16 rxcnt=0;
vu8 timeout=0;
vu8 recok=0;

u32 addr=IAP_APFLASH_START;
	



void UartsendChar(u8 ch)
{
	USART_SendData(HT_USART0, (uint8_t) ch);
	while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET) {}	
}

void UartsendCmd(u8 cmd)
{
	UartsendChar(0xaa);
  UartsendChar(0x55);
	UartsendChar(cmd);
	UartsendChar(0x55);
	UartsendChar(0xaa);
}



void clrdata()
{
	int i;
	for(i=0;i<520;i++)
	rxdata[i]=0;
	rxcnt=0;
	timeout=0;
	recok=0;
}

void GotoUserCode(void)
{
	u32 SP, PC;

	SP = rw(IAP_APFLASH_START);
	if (SP < IAP_APSRAM_START || SP > IAP_APSRAM_END)
	{
	  return; 
	}

	PC = rw(IAP_APFLASH_START + 0x4);
	if (PC < IAP_APFLASH_START || PC > IAP_APFLASH_END)
	{
	  return; 
	}

	NVIC_DisableIRQ(USART0_IRQn);
	USART_DeInit(HT_USART0); 

	NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, IAP_APFLASH_START); 

	IAP_GoCMD(IAP_APFLASH_START);//��λȥ�û�����
}
 

int main(void)
{
	NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 0); 
	delay_init();
	bftm_Init(1000);//1ms
	uart_init(115200);
	LCD_Init();
	
	
	BACK_COLOR=WHITE;
	POINT_COLOR=BLACK;	
	LCD_Clear(WHITE);
	
	LCD_ShowString(50,290,180,20,12,(u8*)"HT32F52352  Cortex-M0+");
	LCD_ShowString(50,305,180,20,12,(u8*)"www.holteksupport.com");
			

	
	
	
	//����Ƿ���ҪIAP
	while(1)
	{
		if (BOOT_MODE != BOOT_MODE_IAP)
		{	
		  LCD_ShowString(0,60,240,20,12,(u8*)"GotoUserCode");
			delay_ms(500);
		  GotoUserCode();
			LCD_ShowString(0,60,240,20,12,(u8*)"NoUserCode,will to bootloader");
			delay_ms(500);
			LCD_ShowString(0,60,240,20,12,(u8*)"                             ");
			break;
		}
		break;
	}
	
	POINT_COLOR=BLUE;
	LCD_ShowString(0,60,240,20,12,(u8*)"Boot loader !");
	POINT_COLOR=BLACK;	
	
	//��ʼIAP
	while(1)
	{		
		if(recok)
		{
			if(rxdata[0]==0xaa && rxdata[1]==0x55 && rxdata[3]==0x55 &&rxdata[4]==0xaa)
			{
				switch(rxdata[2])
				{
					case 0x01://����ͬ��  
						addr=IAP_APFLASH_START;
						UartsendCmd(0x02);//����0x02
						
					break;
					case 0x03://ȥ��д���� & ����flash
					{
						FLASH_OptionByte Option;
						
						FLASH_GetOptionByteStatus(&Option);

						  if (Option.MainSecurity == ENABLE || Option.OptionProtect == ENABLE)
						  {
							//����������޷�IAP
							LCD_ShowString(0,80,240,20,12,(u8*)"Security Protection is enabled    ");
						  }
						  else
						  {
							    int i;
								FLASH_ErasePage(OPTION_BYTE_BASE);//ȥ��д����
								//Option.WriteProtect[0] = 0;
								//Option.WriteProtect[1] = 0;
								//Option.WriteProtect[2] = 0;
								//Option.WriteProtect[3] = 0;
								//FLASH_ProgramOptionByte(&Option);
								LCD_ShowString(0,80,240,20,12,(u8*)"Write Protection disabled.    ");
								delay_ms(500);
								LCD_ShowString(0,80,240,20,12,(u8*)"Erasing Flash! Please Wait....");
								for (i = IAP_APFLASH_START; i < IAP_APFLASH_END; i += FLASH_PAGE_SIZE)
								     FLASH_ErasePage(i);
								
								LCD_ShowString(0,80,240,20,12,(u8*)"Erasing Finfish!              ");
								UartsendCmd(0x04);//����0x04
						  }
						
					}
					break;
					case 0x05://дһ��
					{
						
						int i;
						u32 data=0;
						//д512�ֽ�����
						for(i=0;i<512;i+=4)
						{
						//	data=*(u32*)&rxdata[5+i];
							data=rxdata[8+i];
							data<<=8;
							data+=rxdata[7+i];
							data<<=8;
							data+=rxdata[6+i];
							data<<=8;
							data+=rxdata[5+i];
							FLASH_ProgramWordData(addr+i,data);
							data=0;
						}
						addr+=512;
						UartsendCmd(0x06);//����0x06
					}
					break;
					case 0x07://���
						addr=IAP_APFLASH_START;
						GotoUserCode();//��ת
					break;
				}
			}
			clrdata();
		}
	}
}


// 0XAA 0X55 FUN 0x55 0XAA
/*
		 FUN:01  ����ͬ��      ����
		 FUN:02  �յ�ͬ���ź�  ����

		 FUN:03  ��������Ϣ  ����
		 FUN:04  �������      ����
		 FUN:05  ����flash     ����
	 
*/

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
		if(rxcnt==0)BFTM_EnaCmd(HT_BFTM0, ENABLE);
		rxdata[rxcnt]=Res;
		rxcnt++;
		if(rxcnt>518)rxcnt=0;
 		timeout=0;
    } 

} 
//bftm�жϺ���
void BFTM0_IRQHandler(void)
{
	timeout++;
	if(timeout>20)
	{
		timeout=0;
		recok=1;
		BFTM_EnaCmd(HT_BFTM0, DISABLE);
	}
	BFTM_ClearFlag(HT_BFTM0);
}

