
#include "HT32.h"

//10k 3450k ntc
const u16 temptab[]={  //0 to 100? step 1
3041,3005,2968,2930,2892,2854,2815,2776,2737,2697,
2657,2616,2576,2535,2495,2454,2413,2372,2331,2290,
2249,2209,2168,2128,2088,2048,2008,1969,1930,1891,
1853,1815,1778,1741,1704,1668,1632,1597,1562,1528,
1495,1461,1429,1397,1365,1334,1304,1274,1245,1216,
1188,1160,1133,1107,1081,1056,1031,1006,983,959,
937,915,893,872,851,831,811,792,773,755,
737,719,702,685,669,653,638,623,608,594,
580,566,553,540,528,515,503,492,480,469,
458,448,438,428,418,409,399,390,381,373,
365,
};


void adc_Init(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.PC         = 1;																										//??GPIOA??
  CKCUClock.Bit.AFIO       = 1;																										//??????
  CKCUClock.Bit.ADC        = 1;																										//??ADC??
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV8);																				
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_4, AFIO_FUN_ADC);																
	ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);															
	ADC_RegularGroupConfig(HT_ADC, ONE_SHOT_MODE, 1, 1);														
	ADC_RegularChannelConfig(HT_ADC, ADC_CH_8, 0);																
	
	ADC_Cmd(HT_ADC,ENABLE);
}



u16 getadc(u8 ch)
{
	int sum=0;
	int i;
	ADC_RegularChannelConfig(HT_ADC, ch, 0);	
    for(i=0;i<8;i++)	
	{
		ADC_SoftwareStartConvCmd(HT_ADC,ENABLE);					
		while(ADC_GetFlagStatus(HT_ADC,ADC_FLAG_SINGLE_EOC) != SET);									
		sum += ADC_GetConversionData(HT_ADC,0);
	}		
	return sum>>3;
}


u16 getntctemp(void)
{
	u8 i;
	u16 temp;
	temp=getadc(8);
	if(temp<267)
	{
		return 99;	
	}
	else if(temp>3157)
	{
		return 0;
	}
	else
	{
		for(i=0;i<100;i++)
		{
			if(temptab[i]<temp)	break;
		}
		return i;
	}
}


u16 getlight()
{
	int temp;
	temp=getadc(8);
	return (u16)((4096-temp)*100/4096);
}


