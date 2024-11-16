#ifndef __WAV_H
#define __WAV_H	
#include "ht32.h"
#include "ff.h" 

typedef struct 
{
	u8 chRIFF[4];
	u32 wavlen;
	u8 chWAV[4];
	u8 chFMT[4];
	u16 formart;
	u16 CHnum;
	u32 SampleRate;
	u32 speed;
	u16 ajust;
	u16 SampleBits;
	u8 chDATA[4];
	u32 DATAlen;
}WAV_file;

void audio_init(void);
u8 Playwav(char *file);

#endif
