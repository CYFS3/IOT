
#include "ht32.h"
#include "ht32_usbd_core.h"
#include "ht32_usbd_class.h"
#include "usart.h"


/* Private constants ---------------------------------------------------------------------------------------*/
#define CLASS_REQ_20_SET_LINE_CODING        (0x20)
#define CLASS_REQ_21_GET_LINE_CODING        (0x21)
#define CLASS_REQ_22_SET_CONTROL_LINE_STATE (0x22)

#define CDC_INF_NUM                         (11)
#define CDC_EPT_IN_LENGTH                   (_EP1LEN)
#define CDC_EPT                             (USBD_EPT1)

/* Private function prototypes -----------------------------------------------------------------------------*/
static void USBDClass_MainRoutine(u32 uPara);
static void USBDClass_CDC_ZLPProcess(void);
static void USBDClass_CDC_Rx(void);
s32 USBDClass_CDC_Tx(void);
//static void USBDClass_Reset(u32 uPara);
//static void USBDClass_StartOfFrame(u32 uPara);

//static void USBDClass_Standard_GetDescriptor(USBDCore_Device_TypeDef *pDev);
//static void USBDClass_Standard_SetInterface(USBDCore_Device_TypeDef *pDev);
//static void USBDClass_Standard_GetInterface(USBDCore_Device_TypeDef *pDev);

static void USBDClass_Request(USBDCore_Device_TypeDef *pDev);

static void USBDClass_SetLineCoding(USBDCore_Device_TypeDef *pDev);
static void USBDClass_GetLineCoding(USBDCore_Device_TypeDef *pDev);
static void USBDClass_SetControlLineState(USBDCore_Device_TypeDef *pDev);

static void USBDClass_Endpoint1(USBD_EPTn_Enum EPTn);
static void USBDClass_Endpoint2(USBD_EPTn_Enum EPTn);
static void USBDClass_Endpoint3(USBD_EPTn_Enum EPTn);
//static void USBDClass_Endpoint4(USBD_EPTn_Enum EPTn);
//static void USBDClass_Endpoint5(USBD_EPTn_Enum EPTn);
//static void USBDClass_Endpoint6(USBD_EPTn_Enum EPTn);
//static void USBDClass_Endpoint7(USBD_EPTn_Enum EPTn);

/* Private variables ---------------------------------------------------------------------------------------*/
static vu32 gIsEP3OUT = 0;
static vu32 gEP3Len = 0;

static vu32 gIsCDCEPOUT = 0;
static vu32 gCDCEPOUTLen = 0;
static vu32 gIsCDCEPINEmpty = TRUE;
static u32 gIsZLP_Require = FALSE;
static USBDClass_VCP_LINE_CODING USBDClassVCPLineCoding;

//__ALIGN4 static u8 gInputDataBuffer[64];
__ALIGN4 static u8 gOutputDataBuffer[64];



void USBDClass_Init(USBDCore_Class_TypeDef *pClass)
{
  pClass->CallBack_MainRoutine.func = USBDClass_MainRoutine;
  //pClass->CallBack_MainRoutine.uPara = (u32)NULL;

  //pClass->CallBack_Reset.func = USBDClass_Reset;
  //pClass->CallBack_Reset.uPara = (u32)NULL;

  //pClass->CallBack_StartOfFrame.func = USBDClass_StartOfFrame;
  //pClass->CallBack_StartOfFrame.uPara = (u32)NULL;

  //pClass->CallBack_ClassGetDescriptor = USBDClass_Standard_GetDescriptor
  //pClass->CallBack_ClassSetInterface = USBDClass_Standard_SetInterface;
  //pClass->CallBack_ClassGetInterface = USBDClass_Standard_GetInterface;

  pClass->CallBack_ClassRequest = USBDClass_Request;
  pClass->CallBack_EPTn[1] = USBDClass_Endpoint1;
  pClass->CallBack_EPTn[2] = USBDClass_Endpoint2;
  pClass->CallBack_EPTn[3] = USBDClass_Endpoint3;
  //pClass->CallBack_EPTn[4] = USBDClass_Endpoint4;
  //pClass->CallBack_EPTn[5] = USBDClass_Endpoint5;
  //pClass->CallBack_EPTn[6] = USBDClass_Endpoint6;
  //pClass->CallBack_EPTn[7] = USBDClass_Endpoint7;

  #ifdef RETARGET_IS_USB
  pClass->CallBack_EPTn[RETARGET_RX_EPT] = SERIAL_USBDClass_RXHandler;
  #endif

  return;
}



/* Private functions ---------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  USB Class main routine.
  * @param  uPara: Parameter for Class main routine
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_MainRoutine(u32 uPara)
{
  USBDClass_CDC_ZLPProcess();
  USBDClass_CDC_Rx();
  USBDClass_CDC_Tx();
  return;
}

/*********************************************************************************************************//**
  * @brief  USB Class Report Process for application.
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_CDC_ZLPProcess(void)
{
  u32 *nullptr = 0;
  if (gIsZLP_Require == TRUE && gIsCDCEPINEmpty == TRUE)
  {
    gIsZLP_Require = FALSE;
    gIsCDCEPINEmpty = FALSE;
    USBDCore_EPTWriteINData(USBD_EPT1, (u32 *)nullptr, 0);
  }

  return;
}





/*********************************************************************************************************//**
  * @brief  USB Class Endpoint 1 IN Process for application.
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_CDC_Rx(void)
{
  s32 i;

  /* 检查数据是否接收到 
   gIsCDCEPOUT: 1 - 有数据了. 0 - 没有数据.  
   gOutputDataBuffer[64]: Data buffer 
*/

  //收到的数据发送的实际的串口
  if (gIsCDCEPOUT == 1)
  { 
	for(i=0;i<gCDCEPOUTLen;i++)
	{
		USART_SendData(HT_USART0, gOutputDataBuffer[i]);//向串口1发送数据
		while(USART_GetFlagStatus(HT_USART0,USART_FLAG_TXC)!=SET);//等待发送结束
	}
    gIsCDCEPOUT = 0;
  }
  return;
}

//串口收到的数据 发送到USB
s32 USBDClass_CDC_Tx(void)
{
	u32 len;
	if(USART_RX_STA&RECFINISH)
	{					   
		len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
		if (gIsCDCEPINEmpty == TRUE)
		{
			gIsCDCEPINEmpty = FALSE;	
			
			USART_RX_BUF[len]='\r';
			USART_RX_BUF[len+1]='\n';
			len+=2;
			USBDCore_EPTWriteINData(CDC_EPT, (u32 *)USART_RX_BUF, len);
			
			if (len == CDC_EPT_IN_LENGTH)
			{
				gIsZLP_Require = TRUE; // Data Length = Endpoint maximum Length, need ZLP.
				return 0;
			}
			else
			{
				return -1;
			}
		}
		USART_RX_STA=0; 
	}	
	return -1;	
}


#if 0
/*********************************************************************************************************//**
  * @brief  USB Class Reset.
  * @param  uPara: Parameter for Class Reset.
  * @retval None
  ***********************************************************************************************************/
/*
static void USBDClass_Reset(u32 uPara)
{

}
*/

/*********************************************************************************************************//**
  * @brief  USB Class Start of Frame.
  * @param  uPara: Parameter for Class Start of Frame.
  * @retval None
  ***********************************************************************************************************/
/*
static void USBDClass_StartOfFrame(u32 uPara)
{

}
*/
#endif

/*********************************************************************************************************//**
  * @brief  USB Device Class Request
  * @param  pDev: pointer of USB Device
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_Request(USBDCore_Device_TypeDef *pDev)
{
  u8 USBCmd = *((u8 *)(&(pDev->Request.bRequest)));
  u16 len = *((u16 *)(&(pDev->Request.wLength)));

#ifdef RETARGET_IS_USB
  SERIAL_USBDClass_Request(pDev);
#endif

  switch (USBCmd)
  {
    case CLASS_REQ_22_SET_CONTROL_LINE_STATE:
    {
      if (len == 0)
      {
        USBDClass_SetControlLineState(pDev);
      }
      break;
    }
    case CLASS_REQ_20_SET_LINE_CODING:
    {
      USBDClass_SetLineCoding(pDev);
      break;
    }
    case CLASS_REQ_21_GET_LINE_CODING:
    {
      USBDClass_GetLineCoding(pDev);
      break;
    }
    // others not support
    default:
    {
      break;
    }
  }
  return;
}

/*********************************************************************************************************//**
  * @brief  USB Device Class Request - Set Line Coding
  * @param  pDev: pointer of USB Device
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_SetLineCoding(USBDCore_Device_TypeDef *pDev)
{
  pDev->Transfer.pData = (uc8*)&USBDClassVCPLineCoding;
  pDev->Transfer.sByteLength = (sizeof(USBDClassVCPLineCoding) > pDev->Request.wLength) ? (pDev->Request.wLength) : (sizeof(USBDClassVCPLineCoding));
  pDev->Transfer.Action = USB_ACTION_DATAOUT;
  return;
}

/*********************************************************************************************************//**
  * @brief  USB Device Class Request - Get Line Coding
  * @param  pDev: pointer of USB Device
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_GetLineCoding(USBDCore_Device_TypeDef *pDev)
{
  pDev->Transfer.pData = (uc8*)&USBDClassVCPLineCoding;
  pDev->Transfer.sByteLength = (sizeof(USBDClassVCPLineCoding) > pDev->Request.wLength) ? (pDev->Request.wLength) : (sizeof(USBDClassVCPLineCoding));
  pDev->Transfer.Action = USB_ACTION_DATAIN;
  return;
}

/*********************************************************************************************************//**
  * @brief  USB Device Class Request - Set Control Line State
  * @param  pDev: pointer of USB Device
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_SetControlLineState(USBDCore_Device_TypeDef *pDev)
{
  pDev->Transfer.pData = 0;
  pDev->Transfer.sByteLength = 0;
  pDev->Transfer.Action = USB_ACTION_DATAOUT;

  return;
}

/*********************************************************************************************************//**
  * @brief  USB Class Endpoint handler
  * @param  EPTn: USB Endpoint number
  *         @arg USBD_EPT0 ~ USBD_EPT7
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_Endpoint1(USBD_EPTn_Enum EPTn)
{
  gIsCDCEPINEmpty = TRUE;
  return;
}

/*********************************************************************************************************//**
  * @brief  USB Class Endpoint handler
  * @param  EPTn: USB Endpoint number
  *         @arg USBD_EPT0 ~ USBD_EPT7
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_Endpoint2(USBD_EPTn_Enum EPTn)
{
  return;
}

/*********************************************************************************************************//**
  * @brief  USB Class Endpoint handler
  * @param  EPTn: USB Endpoint number
  *         @arg USBD_EPT0 ~ USBD_EPT7
  * @retval None
  ***********************************************************************************************************/
static void USBDClass_Endpoint3(USBD_EPTn_Enum EPTn)
{
  /* Read Receive data.                                                                                     */
  gCDCEPOUTLen = USBDCore_EPTReadOUTData(EPTn, (u32 *)gOutputDataBuffer, 64);
  gIsCDCEPOUT = 1;

  __DBG_USBPrintf("%06ld EP3OUT\t[%02d]", ++__DBG_USBCount, (int)gCDCEPOUTLen);
  __DBG_USBDump((uc8 *)gOutputDataBuffer, gCDCEPOUTLen);

  return;
}

/*********************************************************************************************************//**
  * @brief  USB Class Endpoint handler
  * @param  EPTn: USB Endpoint number
  *         @arg USBD_EPT0 ~ USBD_EPT7
  * @retval None
  ***********************************************************************************************************/
/*
static void USBDClass_Endpoint4(USBD_EPTn_Enum EPTn)
{
  return;
}
*/

/*********************************************************************************************************//**
  * @brief  USB Class Endpoint handler
  * @param  EPTn: USB Endpoint number
  *         @arg USBD_EPT0 ~ USBD_EPT7
  * @retval None
  ***********************************************************************************************************/
/*
static void USBDClass_Endpoint5(USBD_EPTn_Enum EPTn)
{
  return;
}
*/

/*********************************************************************************************************//**
  * @brief  USB Class Endpoint handler
  * @param  EPTn: USB Endpoint number
  *         @arg USBD_EPT0 ~ USBD_EPT7
  * @retval None
  ***********************************************************************************************************/
/*
static void USBDClass_Endpoint6(USBD_EPTn_Enum EPTn)
{
  return;
}
*/

/*********************************************************************************************************//**
  * @brief  USB Class Endpoint handler
  * @param  EPTn: USB Endpoint number
  *         @arg USBD_EPT0 ~ USBD_EPT7
  * @retval None
  ***********************************************************************************************************/
/*
static void USBDClass_Endpoint7(USBD_EPTn_Enum EPTn)
{
  return;
}
*/


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
