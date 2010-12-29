/*!
 * \file  time.h
 * \brief Time functions
 *
 * time.h defines standard time functions that are used by kernel.
 *
 * \sa hardware/bios/rtc.h
 */

#ifndef _TIME_H
#define	_TIME_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef NULL
  #define NULL
#endif

/*!
 * \struct tm
 * \brief  struct tm holds information about time
 *
 * struct tm holds information about time, which are returned by some time-handling
 * functions used by kernel (ie. hardware/bios/rtc.h).
 *
 * \sa hardware/bios/rtc.h
 */
typedef struct tm {
  int tm_sec;   /*!< seconds (from 0 to 61)                 */
  int tm_min;   /*!< minutes (from 0 to 59)                 */
  int tm_hour;  /*!< hour (from 0 to 23)                    */
  int tm_mday;  /*!< day of month (from 1 to 31)            */
  int tm_mon;   /*!< months since January (from 0 to 11)    */
  int tm_year;  /*!< years since 1900 (from 0        )      */
  int tm_wday;  /*!< days since last Sunday (from 0 to 6)   */
  int tm_yday;  /*!< days since January 1 (from 0 to 365)   */
  int tm_isdst; /*!< reater than zero if Daylight Saving Time is in effect,
                 * zero if Daylight Saving Time is not in effect, and less than
                 * zero if the information is not available */
} tm, *ptm;

#ifdef	__cplusplus
}
#endif

#endif

