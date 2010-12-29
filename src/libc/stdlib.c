#include <stdlib.h>
#include <string.h>
#include <math.h>

int abs(int x) {
  if (x >= 0)
    return x;
  else
    return -x;
}

long int absl(long int x) {
  if (x >= 0)
    return x;
  else
    return -x;
}

long long int absll(long int x) {
  if (x >= 0)
    return x;
  else
    return -x;
}

char* uitoa(unsigned int value, char* buf, int base) {
  unsigned int r, v, vdivb, i;
  size_t j;

  v = value;
  j = 0;
  do {
    vdivb = v / base;
    r = v - (vdivb * base); // notice, that 123 / 10 = 12 :)
    v = vdivb;
    if (base > 10 && r > 9)
      buf[j] = r + 87;  // 87 = 'a' - 9
    else
      buf[j] = r + '0';
    j++;
  } while (1 <= v);

  char rev[j + 1];
  memset(rev, 0, j + 1);
  for (i = 0; i < j; i++)
    rev[i] = buf[j - i - 1];

  strcpy(buf, rev);
  return buf;
}

char* itoa(int value, char* buf, int base) {
  int r, v, vdivb;
  size_t j, i;

  v = value;
  j = 0;
  do {
    vdivb = v / base;
    r = v - (vdivb * base); // notice, that 123 / 10 = 12 :)
    v = vdivb;
    if (base > 10 && r > 9)
      buf[j] = r + 87;  // 87 = 'a' - 9
    else
      buf[j] = r + '0';
    j++;
  } while (1 <= v);
  
  if (base == 10 && value < 0) {
    buf[j] = '-';
    j++;
  }

  char rev[j + 1];
  memclr(rev, j + 1);
  for (i = 0; i < j; i++)
    rev[i] = buf[j - i - 1];

  strcpy(buf, rev);
  return buf;
}

char* dtoa(double value, char* buf, int precision) {
  char buf0[32], buf1[4];
  itoa((int) value, buf0, 10);
  strcat(buf0, ".");

  int i;
  double rest = value - floor(value);
  for (i = 0; i < precision; i++) {
    rest *= 10.0;
    itoa((int) rest, buf1, 10);
    strcat(buf0, buf1);
    rest -= floor(rest);
  }
  strcpy(buf, buf0);
  return buf;
}