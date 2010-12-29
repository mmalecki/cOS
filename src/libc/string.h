#ifndef _STRING_H
#define	_STRING_H

#ifndef NULL
  #define NULL 0
#endif

#ifndef SIZE_T
  typedef unsigned int size_t;
  #define SIZE_T
#endif

size_t strlen(const char* str);
void* memset(void* ptr, int value, size_t num);
void* memmove(void* destination, const void* source, size_t n);
void* memclr(void* ptr, size_t num);
void* memchr(const void* ptr, int value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);
char* strcat(char* destination, const char* source);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t num);
char* strcpy(char* destination, const char* source);
char* strchr(const char* str, int character);
char* strstr(const char* str1, const char* str2);

#endif

