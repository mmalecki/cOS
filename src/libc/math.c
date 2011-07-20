#include <math.h>
#include <stdlib.h>
#include "hardware/cpu/cpu.h"
#include "macros.h"
#include "types.h"
#include "math/__factorial.h"
#include "kernel/screen.h"

#ifndef LOG_PRECISION
  #define LOG_PRECISION 32
#endif

extern uint32 cpu_ext_mask;

int is_pow2(uint x) {
  int r0, r1;
  asm(
    "bsf %2, %0 \n\t"
    "bsr %2, %1 \n\t"
  : "=r" (r0), "=r" (r1) : "r" (x) :);
  return (r0 == r1) ? r1 : FALSE;
}

// pow_* implement a simple, iterations exponentation by squaring method
double pow_di(double base, int exponent) {
  if (exponent == 0 || base == 1.0)
    return 1;

  register int i, n = abs(exponent);
  register double r = 1;

  for (i = 31; i >= 0; i--) {
    if (IS_BIT_SET(n, i))
      r *= r * base; // r = r * r * base
    else
      r *= r;
  }
  return (exponent >= 0) ? r : 1 / r;
}

long double pow_ldi(long double base, int exponent) {
  if (exponent == 0 || base == 1.0)
    return 1;

  int i, n = abs(exponent);
  register long double r = 1;

  for (i = 31; i >= 0; i--) {
    if (IS_BIT_SET(n, i))
      r *= r * base; // r = r * r * base
    else
      r *= r;
  }
  return (exponent >= 0) ? r : 1 / r;
}

double pow_ii(int base, int exponent) {
  if (exponent == 0 || base == 1)
    return 1;

  register int r = 1;
  register int i, n = abs(exponent);

  if ((i = is_pow2(n)) > 0)
    r = base << ((n - 1) * i);
  else {
    for (i = 31; i >= 0; i--) {
      if (IS_BIT_SET(n, i))
        r *= r * base; // r = r * r * base
      else
        r *= r;
    }
  }
  return (exponent >= 0) ? ((double) r) : (1 / ((double) r));
}

int pow_iui(int base, uint exponent) {
  if (exponent == 0 || base == 1)
    return 1;

  register int i, r = 1;

  if ((i = is_pow2(exponent)) > 0)
    return base << ((exponent - 1) * i);
  else {
    for (i = 31; i >= 0; i--) {
      if (IS_BIT_SET(exponent, i))
        r *= r * base; // r = r * r * base
      else
        r *= r;
    }
    return r;
  }
}

double pow(double base, double exponent) {
  return exp(exponent * log(base));
}

double exp(double x) {
  int i;
  register double r = 0;
  for (i = 0; i < FACTORIALS; i++)
    r += pow_di(x, i) / ((double) __factorial[i]);
  return r;
}

double log(double x) {
  if (x == 1)
    return 0;
  int i;
  register double r = 0;
  for (i = 1; i <= LOG_PRECISION; i++)
    r += (1/((2 * ((double) i)) - 1)) * pow_di((x - 1) / (x + 1), (2 * i) - 1);
  /*
   * In docs*, there's a cool, graphical presentation of formula above, altough:
   *   
   *            _inf__
   *            \         1       / x - 1 \ 2n - 1
   * ln x = 2 *  |    -------- * ( ------- )
   *            /_____ 2n - 1     \ x + 1 /
   *             n = 1
   *
   * Let infinity be LOG_PRECISION, in most cases 32 :).
   * As it comes from Taylors formulas.
   *
   * * If there are no docs about it, there probably won't be any in future. I'm lazy.
   */
  return 2 * r;
}

double fabs(double x) {
  if (x >= 0)
    return x;
  else
    return -x;
}

double ceil(double x) {
  return (double) trunc(x) + 1.0;
}

double floor(double x) {
  return (double) trunc(x);
}


int trunc(float x) {
  #ifdef __SSE__
  if (CPU_EXT(SSE)) {
    register int r;
    asm("cvttss2si %1, %0" : "=r" (r) : "m" (x));
    return r;
  }
  else
  #endif
    return (int) x;
}

double round(double x) {
  register double fl = floor(x);
  if (x - fl >= 0.5)
    return fl + 1.0;
  else
    return fl;
}

long int lround(double x) {
  return (long int) round(x);
}