#include "rtc.h"
#include <time.h>
#include "macros.h"
#include "types.h"
#include "kernel/interrupts/int.h"
#include "cmos.h"

uint8 rtc_reg_B;

void rtc_init() {
  rtc_reg_B = cmos_read(RTC_FORMAT_REG);
}

uint8 rtc_convert(uint8 val) {
  if (!(IS_FLAG_SET(rtc_reg_B, RTC_BIN))) {
    uint8 a = val & 0xF;
    uint8 b = (val >> 4) & 0xF;
    val = a + (b * 10);
  }
  return val;
}

uint8 rtc_convert_hour(uint8 h) {
  h = (!(IS_FLAG_SET(rtc_reg_B, RTC_24H)) && IS_FLAG_SET(h, RTC_12H_PM)) ?
     ((h & 0xF) + 12) : h;
  return rtc_convert(h);
}

uint8 rtc_get_hour() {
  return rtc_convert_hour(cmos_read(RTC_HOUR_REG));
}

uint8 rtc_get_minute() {
  return rtc_convert(cmos_read(RTC_MINUTE_REG));
}

uint8 rtc_get_second() {
  return rtc_convert(cmos_read(RTC_SECOND_REG));
}

uint8 rtc_get_day() {
  return rtc_convert(cmos_read(RTC_DAY_REG));
}

uint8 rtc_get_month() {
  return rtc_convert(cmos_read(RTC_MONTH_REG));
}

uint8 rtc_get_year() {
  return rtc_convert(cmos_read(RTC_YEAR_REG));
}

void rtc_get_time(tm* time) {
  int_disable();
  time->tm_hour = rtc_convert_hour(cmos_reads(RTC_HOUR_REG));
  time->tm_min  = rtc_convert(cmos_reads(RTC_MINUTE_REG));
  time->tm_sec  = rtc_convert(cmos_reads(RTC_SECOND_REG));
  time->tm_year = rtc_convert(cmos_reads(RTC_YEAR_REG));
  time->tm_mday = rtc_convert(cmos_reads(RTC_DAY_REG));
  time->tm_mon  = rtc_convert(cmos_reads(RTC_MONTH_REG));
  int_enable();
}
