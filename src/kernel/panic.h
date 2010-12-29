/* 
 * File:   panic.h
 * Author: Maciej
 *
 * Created on 17 sierpień 2010, 16:34
 */

#ifndef _PANIC_H
  #define	_PANIC_H

  #ifdef	__cplusplus
extern "C" {
  #endif

#define kpanic(msg) _kpanic(msg, __FILE__, __LINE__)

void _kpanic(char* msg, char* file, int line);

  #ifdef	__cplusplus
}
  #endif

#endif	/* _PANIC_H */

