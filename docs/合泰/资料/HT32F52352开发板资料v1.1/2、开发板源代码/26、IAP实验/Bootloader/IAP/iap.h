#ifndef _IAP_H_
#define _IAP_H_

#include "ht32.h"

#define IAP_APFLASH_SIZE        (512*255)
#define IAP_APFLASH_END         (IAP_APFLASH_SIZE - 1)

#define IAP_APSRAM_SIZE         (1024*16)
#define IAP_APSRAM_END          (HT_SRAM_BASE + IAP_APSRAM_SIZE)

#define IAP_CODE_SIZE                 (1024 * 20)//Ç°20kÓÃ×÷bootloader

#define IAP_APFLASH_START             (IAP_CODE_SIZE + 0)
#define IAP_APSRAM_START              (HT_SRAM_BASE)

#define BOOT_MODE_AP                  (0x55AAFAF0)
#define BOOT_MODE_IAP                 (0x55AAFAF5)
#define BOOT_MODE                     ((*((u32 volatile *) (HT_SRAM_BASE+0x4000-4))))

void IAP_GoCMD(u32 address);
void GotoUserCode(void);

#endif 





