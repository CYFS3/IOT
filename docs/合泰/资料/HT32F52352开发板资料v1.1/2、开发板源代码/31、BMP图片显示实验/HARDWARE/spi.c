#include "spi.h"
#include "ht32.h"




void SPI1_Init(void)
{
	
  SPI_InitTypeDef  SPI_InitStructure;

  /*  Enable AFIO & SPI SEL pin port & SPI clock                                                            */
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.PC      = 1;
  CKCUClock.Bit.SPI1    = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);


  /*  Configure SPI SCK pin, SPI MISO pin, SPI MOSI pin                                                     */
  AFIO_GPxConfig(GPIO_PC, GPIO_PIN_5, AFIO_FUN_SPI);
  AFIO_GPxConfig(GPIO_PC, GPIO_PIN_8, AFIO_FUN_SPI);
  AFIO_GPxConfig(GPIO_PC, GPIO_PIN_9, AFIO_FUN_SPI);

  /*  SPI Configuration                                                                                     */
  SPI_InitStructure.SPI_Mode = SPI_MASTER;
  SPI_InitStructure.SPI_FIFO = SPI_FIFO_DISABLE;
  SPI_InitStructure.SPI_DataLength = SPI_DATALENGTH_8;
  SPI_InitStructure.SPI_SELMode = SPI_SEL_SOFTWARE;
  SPI_InitStructure.SPI_SELPolarity = SPI_SELPOLARITY_LOW;
  SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_HIGH;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_SECOND;
  SPI_InitStructure.SPI_RxFIFOTriggerLevel = 0;
  SPI_InitStructure.SPI_TxFIFOTriggerLevel = 0;
  SPI_InitStructure.SPI_ClockPrescaler = 2;
  SPI_Init(HT_SPI1, &SPI_InitStructure);

  SPI_SELOutputCmd(HT_SPI1, ENABLE);

  SPI_Cmd(HT_SPI1, ENABLE);
  

	
  SPI1_ReadWriteByte(0xff);//启动传输		 
 

}   
//SPI 速度设置函数
//SPI_GUADTIME_1_SCK to SPI_GUADTIME_16_SCK
  
void SPI1_SetSpeed(u32 Guard_Time)
{
    SPI_GUARDTConfig(HT_SPI1,Guard_Time);
	SPI_Cmd(HT_SPI1, ENABLE);
} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		
  u8 retry=0;
  /* Loop while Tx buffer register is empty                                                                 */
  while (!SPI_GetFlagStatus(HT_SPI1, SPI_FLAG_TXBE))
  {
	retry++;
	if(retry>200)return 0;
  }

  /* Send byte through the SPIx peripheral                                                                  */
  SPI_SendData(HT_SPI1, TxData);

  /* Loop while Rx is not empty                                                                             */
  while (!SPI_GetFlagStatus(HT_SPI1, SPI_FLAG_RXBNE))
  {
	retry++;
	if(retry>200)return 0;
  }

  /* Return the byte read from the SPI                                                                      */
  return SPI_ReceiveData(HT_SPI1);	
  
}































