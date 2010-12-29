#ifndef STDLIB_H
#define	STDLIB_H

#ifndef NULL
  #define NULL 0
#endif

#ifndef SIZE_T
  typedef unsigned int size_t;
  #define SIZE_T
#endif

int abs(int x);
long int absl(long int x);
long long int absll(long int x);

char* uitoa(unsigned int value, char* buf, int base);
char* itoa(int value, char* buf, int base);
// ftoa exists actually only in AVR-GCC, though it may be useful here
char* dtoa(double value, char* buf, int precision);

#endif	/* STDLIB_H */

