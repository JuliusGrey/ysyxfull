#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint32_t timelow ;
  uint32_t timehigh ;
  timelow = inl(RTC_ADDR);
  timehigh =inl(RTC_ADDR + 4);
  uptime->us = (uint64_t)timelow | (uint64_t)timehigh << 32;
  // printf("uptime->us is %ld\n",uptime->us);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 1111;
  rtc->year   = 1900;
}
