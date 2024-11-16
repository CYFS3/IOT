#ifndef __RTC_H
#define __RTC_H
#include "ht32.h"



typedef struct
{
  u32 year;
  u32 month;
  u32 day;
  u32 hour;
  u32 minute;
  u32 second;
}Time_T;

extern vu8 CK_SECOND_Flag;

void RTC_Configuration(void);
u8 AP_Time_Count(Time_T* CurrentTime);
u8 AP_Time_Adjust(Time_T* AdjustTime);

void rtc_init(void);

#endif
