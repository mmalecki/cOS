#ifndef _RTC_H
#define	_RTC_H

#include "types.h"
#include <time.h>

#define RTC_FORMAT_REG 0x0B
#define RTC_HOUR_REG   0x04
#define RTC_MINUTE_REG 0x02
#define RTC_SECOND_REG 0x00
#define RTC_DAY_REG    0x07
#define RTC_MONTH_REG  0x08
#define RTC_YEAR_REG   0x09

#define RTC_BIN        1 << 2
#define RTC_24H        1 << 1
#define RTC_12H_PM     1 << 7

uint8 rtc_get_hour();
uint8 rtc_get_minute();
uint8 rtc_get_second();
uint8 rtc_get_day();
uint8 rtc_get_month();
uint8 rtc_get_year();
void rtc_get_time(tm* time);

#endif

