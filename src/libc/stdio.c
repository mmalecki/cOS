#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* get_int_str(char* buf, char format, size_t width, void* val) {
  char ibuf[64];
  size_t i;
  
  if (format == 'd' || format == 'i')
    itoa(*((int*) val), ibuf, 10);
  else if (format == 'x' || format == 'X') {
    uitoa(*((unsigned int*) val), ibuf, 16);
    if (format == 'X') {
      // upper case it!
    }
  }
  else if (format == 'u')
    uitoa(*((unsigned int*) val), ibuf, 10);
  
  i = strlen(ibuf);
  if ((width != 0) && (i < width)) {
    while (i++ < width)
      *(buf++) = '0';
  }
  i = 0;
  while (ibuf[i] != 0) 
    *(buf++) = ibuf[i++];
  *buf = 0;
  return buf;
}

int sprintf(register char* str, const char * format, ...) {
  char* begin = str;
  char** arg = (char**) &format;
  arg++;

  char* p;
  size_t i, width;
  char buf[64];
  int b;

  do {
    if (*format == '%') {
      width = 0;
      format++;

      b = 1;
      while ((*format >= '0') && (*format <= '9')) {
        width = (*format - '0') * b;
        b *= 10;
        ++format;
      }

      switch (*format) {
        case '%': // percent sign
          *(str++) = '%';
          break;

        case 's': // string
          p = *arg++;
          if (!p)
            p = "(null)";
          if (width != 0) {
            for (i = 0; i < width && (*p != 0); i++)
              *(str++) = *(p++);
          }
          else {
            while (*p != 0)
              *(str++) = *(p++);
          }
          break;

        case 'i':
        case 'd': // signed decimal integer
        case 'u': // unsigned decimal integer
        case 'x': // unsigned hexdecimal integer
          get_int_str(buf, *format, width, arg++);
          i = 0;
          while (buf[i] != 0)
            *(str++) = buf[i++];
          break;

        default: // ekhm...
          break;
      }
    }
    else {
      *(str++) = *format;
    }
  } while (*(++format) != 0);
  *str = 0;
  return str - begin;
}