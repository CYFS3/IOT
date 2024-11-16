
#include "HT32.h"
#include "rtc.h"

u8 Day_Per_Month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

vu8 CK_SECOND_Flag = 0;

void rtc_init() 
{	
  //使能备份域时钟 等待可以被操作
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.BKP        = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
  if (PWRCU_CheckReadyAccessed() != PWRCU_OK)
  {
    while (1);
  }

  NVIC_EnableIRQ(RTC_IRQn);
 
}


//判断是否是闰年
bool IsLeapYear(u32 year)
{
  if (((year % 4 == 0) && (year % 100 != 0) ) || (year % 400 == 0) )
    return TRUE;
  else
    return FALSE;
}


//调节时间
u8 AP_Time_Adjust(Time_T* AdjustTime)
{
  u32 i, temp, secsum = 0;

  temp = AdjustTime->year - 1;
  for (i = 0; i < (AdjustTime->year - 2014); i++)
  {
    if (IsLeapYear(temp--) == TRUE)
    {
      secsum += (366 * 86400);
    }
    else
    {
      secsum += (365 * 86400);
    }
  }

  temp = 1;
  for (i = 0; i < (AdjustTime->month - 1); i++)
  {
    if (temp == 2)
    {
      if (IsLeapYear(AdjustTime->year) == TRUE)
        secsum += (29 * 86400);
      else
        secsum += (28 * 86400);
    }
    else
    {
      secsum += (Day_Per_Month[temp] * 86400);
    }
    temp++;
  }

  secsum += ((AdjustTime->day - 1) * 86400);
  secsum += (AdjustTime->hour * 3600 );
  secsum += (AdjustTime->minute * 60);
  secsum += (AdjustTime->second);

  PWRCU_WriteBackupRegister((PWRCU_BAKREG_Enum) PWRCU_BAKREG_1, secsum);
  if (PWRCU_ReadBackupRegister((PWRCU_BAKREG_Enum) PWRCU_BAKREG_1) != secsum)
  {
    return 0;
  }

  return 1;
}


//计算当前时间
u8 AP_Time_Count(Time_T* CurrentTime)
{
  u32 i, secsum = 0, temp = 0;

  secsum = PWRCU_ReadBackupRegister((PWRCU_BAKREG_Enum) PWRCU_BAKREG_1);
  secsum += RTC_GetCounter();

  temp = 0;
  while (secsum >= (365 * 86400))
  {
    if (IsLeapYear(2014 + temp))
    {
      if (secsum >= (366 * 86400))
      {
        temp++;
        secsum -= (366 * 86400);
      }
      else
      {
        break;
      }
    }
    else
    {
      temp++;
      secsum -= (365 * 86400);
    }
  }
  CurrentTime->year = 2014 + temp;

  for (i = 1; i <= 12; i++)
  {
    if (secsum >= (Day_Per_Month[i] * 86400))
    {
      if (i == 2)  // February
      {
        if (IsLeapYear(CurrentTime->year))
        {
          if (secsum >= (29 * 86400))
            secsum -= (29 * 86400);
          else
            break;
        }
        else
        {
          secsum -= (28 * 86400);
        }
      }
      else
      {
        secsum -= (Day_Per_Month[i] * 86400);
      }
    }
    else
    {
      break;
    }
  }
  CurrentTime->month = i;

  CurrentTime->day = secsum / 86400 + 1;
  secsum -= ((CurrentTime->day - 1) * 86400);

  CurrentTime->hour = secsum / 3600;
  CurrentTime->minute = (secsum % 3600) / 60;
  CurrentTime->second = (secsum % 3600) % 60;

  return 1;
}


void RTC_Configuration(void)
{
  
  PWRCU_DeInit();

  //检查外部低速振荡器是否存在
  if(GetBit_BB((u32)(&HT_CKCU->GCSR), 4))
  {
    //配置外部32.768k振荡器
    RTC_LSESMConfig(RTC_LSESM_FAST);
    RTC_LSECmd(ENABLE);
    while (CKCU_GetClockReadyStatus(CKCU_FLAG_LSERDY) == RESET);
    RTC_ClockSourceConfig(RTC_SRC_LSE);
  }
  else
  {
    RTC_ClockSourceConfig(RTC_SRC_LSI);
  }
  RTC_IntConfig(RTC_INT_CSEC, ENABLE);
  RTC_SetPrescaler(RTC_RPRE_32768);

                                                            
  RTC_CMPCLRCmd(ENABLE);
}

//RTC中断
void RTC_IRQHandler(void)
{
  u8 bFlags;

  bFlags = RTC_GetFlagStatus();
  if (bFlags & 0x1)
  {
    //1s更新标志位
    CK_SECOND_Flag = 1;
  }

}


