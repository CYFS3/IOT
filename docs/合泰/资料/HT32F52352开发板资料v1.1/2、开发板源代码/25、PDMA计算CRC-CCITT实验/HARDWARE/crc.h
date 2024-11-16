#ifndef __CRC_H
#define __CRC_H 			   
#include "ht32.h"

void CRC_init(void);

u16 crc16_fast(u16 crc, u8* buffer, u32 length);
u16 crc_ccitt_fast(u16 crc, u8 *buffer, u32 length);

#endif



