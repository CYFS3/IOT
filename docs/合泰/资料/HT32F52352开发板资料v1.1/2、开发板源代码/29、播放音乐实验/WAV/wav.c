#include "wav.h"
#include "delay.h"
#include "ht32.h"
#include "bftm.h"
#include "mctm.h"
WAV_file wav;

u16 CHanalnum,Bitnum;
float volume=1;
u8 wav_buf[512];
u8 wav_buf1[512];
u16 DApc;
u16 DACdone;




void BFTM0_IRQHandler(void)
{                                                      
  u16 temp;
  
		
  if(CHanalnum==1)
  {
		if(Bitnum==8)
		{
			temp=wav_buf[DApc]*volume;
			mctm_pwm(temp);                   
			DApc++;
		}
		else if(Bitnum==16)                    
		{
			//temp=(((u8)(wav_buf[DApc+1]-0x80)<<3)|(wav_buf[DApc]>>5))*volume; //11位数据
			//mctm_pwm(temp);   
			DApc+=2;                     
		 }
  }
  else if(CHanalnum==2)
  {
		 if(Bitnum==8)
		 {
			temp=wav_buf[DApc]*volume;
			mctm_pwm(temp);     
			DApc++;
			DApc++;                 
		 }
		 else if(Bitnum==16)
		 {    
			//temp=(((u8)(wav_buf[DApc+1]-0x80)<<3)|(wav_buf[DApc]>>5))*volume; //11位数据
			//mctm_pwm(temp);   
			DApc+=2;            
			DApc+=2;                       
		 }
  }           
  if(DApc>=512)
  {
	DApc=0;
	DACdone=1;
  }                                                                                                        
   BFTM_ClearFlag(HT_BFTM0);       
}


void audio_init()
{
	mctm_pwm_Init(187500);
	mctm_pwm(127);
}



u8 Check_Ifo(u8* pbuf1,u8* pbuf2)
{
    u8 i;
    for(i=0;i<4;i++)
	{
		  if(pbuf1[i]!=pbuf2[i])
		  return 1;
	}
    return 0;
}
 
u32 Get_num(u8* pbuf,u8 len)
{
	u32 num;
	if(len==2)num=(pbuf[1]<<8)|pbuf[0];
	else if(len==4)num=(pbuf[3]<<24)|(pbuf[2]<<16)|(pbuf[1]<<8)|pbuf[0];
	return num;
}

u8 WAV_Init(u8* wav_buf)
{
       if(Check_Ifo(wav_buf,(u8*)"RIFF"))return 1; 
       wav.wavlen=Get_num(wav_buf+4,4);
       wav.formart=Get_num(wav_buf+20,2);
       wav.CHnum=Get_num(wav_buf+22,2);
       CHanalnum=wav.CHnum;
       wav.SampleRate=Get_num(wav_buf+24,4);
       wav.speed=Get_num(wav_buf+28,4);
       wav.ajust=Get_num(wav_buf+32,2);
       wav.SampleBits=Get_num(wav_buf+34,2);
       Bitnum=wav.SampleBits;
       wav.DATAlen=Get_num(wav_buf+40,4);
       return 0;
}

void mycharcpy(u8*a,u8*b,u32 len)
{
	u32 i;
	for(i=0;i<len;i++)
		a[i]=b[i];
}

u8 Playwav(char *file)
{
       FIL fwav;
       FRESULT Res;
       UINT BR;
       u32 i;
       u32 times;
       Res = f_open(&fwav, file, FA_OPEN_EXISTING | FA_READ);
       if(Res == FR_OK)
	   {
			Res = f_read(&fwav, wav_buf, sizeof(wav_buf), &BR);
			if(Res==FR_OK)
			{
				 WAV_Init(wav_buf);
				 DACdone=0;         

				
				 Res = f_read(&fwav, wav_buf, 512, &BR);
				 bftm_Init(1000000/wav.SampleRate);
				 times=wav.wavlen/512;        
			     times--;	
				
				 for(i=0;i<times;i++)
				 {    
					  Res = f_read(&fwav, wav_buf1, 512, &BR);
					  while(!DACdone) ; 
					  DACdone=0;
					  mycharcpy(wav_buf,wav_buf1,512);
				 }
		  }
		  BFTM_EnaCmd(HT_BFTM0, DISABLE);
		  f_close(&fwav);
       }
       return 0;
}

