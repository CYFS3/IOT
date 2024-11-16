/*********************************************************************************************************//**
 * @file    USBD/Mass_Storage/sd_disk.h
 * @version $Rev:: 330          $
 * @date    $Date:: 2016-04-14 #$
 * @brief   The header file of SD Disk.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __SD_DISK_H
#define __SD_DISK_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "usb_bulk_only_transport.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USBD_Examples USBD
  * @{
  */

/** @addtogroup Mass_Storage
  * @{
  */
/* Exported types ------------------------------------------------------------------------------------------*/
typedef struct
{
  u8  CSDStruct;            /* CSD structure                                                                */
  u8  SysSpecVersion;       /* System specification version                                                 */
  u8  Reserved1;            /* Reserved                                                                     */
  u8  TAAC;                 /* Data read access-time 1                                                      */
  u8  NSAC;                 /* Data read access-time 2 in CLK cycles                                        */
  u8  MaxBusClkFrec;        /* Max. bus clock frequency                                                     */
  u16 CardComdClasses;      /* Card command classes                                                         */
  u8  RdBlockLen;           /* Max. read data block length                                                  */
  u8  PartBlockRead;        /* Partial blocks for read allowed                                              */
  u8  WrBlockMisalign;      /* Write block misalignment                                                     */
  u8  RdBlockMisalign;      /* Read block misalignment                                                      */
  u8  DSRImpl;              /* DSR implemented                                                              */
  u8  Reserved2;            /* Reserved                                                                     */
  u16 DeviceSize;           /* Device Size                                                                  */
  u8  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min                                                  */
  u8  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max                                                  */
  u8  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min                                                 */
  u8  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max                                                 */
  u8  DeviceSizeMul;        /* Device size multiplier                                                       */
  u8  EraseGrSize;          /* Erase group size                                                             */
  u8  EraseGrMul;           /* Erase group size multiplier                                                  */
  u8  WrProtectGrSize;      /* Write protect group size                                                     */
  u8  WrProtectGrEnable;    /* Write protect group enable                                                   */
  u8  ManDeflECC;           /* Manufacturer default ECC                                                     */
  u8  WrSpeedFact;          /* Write speed factor                                                           */
  u8  MaxWrBlockLen;        /* Max. write data block length                                                 */
  u8  WriteBlockPaPartial;  /* Partial blocks for write allowed                                             */
  u8  Reserved3;            /* Reserved                                                                     */
  u8  ContentProtectAppli;  /* Content protection application                                               */
  u8  FileFormatGrouop;     /* File format group                                                            */
  u8  CopyFlag;             /* Copy flag (OTP)                                                              */
  u8  PermWrProtect;        /* Permanent write protection                                                   */
  u8  TempWrProtect;        /* Temporary write protection                                                   */
  u8  FileFormat;           /* File Format                                                                  */
  u8  ECC;                  /* ECC code                                                                     */
  u8  msd_CRC;              /* CRC                                                                          */
  u8  Reserved4;            /* always 1                                                                     */
} SD_CSD_TypeDef;


#define SDHC_SUPPORT                      (1)

/* Settings ------------------------------------------------------------------------------------------------*/
#define SDDISK_NUM             (1)

#define SDDISK0_ID             (0)
#define SDDISK0_BlockSize      (512)
//#define SDDISK1_ID             (1)
//#define SDDISK1_BlockSize      (512)

#define SDISK_DEBUG            (0)

/* Exported functions --------------------------------------------------------------------------------------*/
u32 SDDISK_Read(u32 uId, u8 **pBuffer, u32 uAddress, u32 uLength);
u32 SDDISK_Write(u32 uId, u8 *pBuffer, u32 uAddress, u32 uLength);
u32 SDDISK_GetStatus(u32 id, BOT_MediaSize_Typedef *pSizeInfo);
void SDDISK_Unmount(u32 uId);



#endif /* __SD_DISK_H --------------------------------------------------------------------------------------*/
