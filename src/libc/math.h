/*!
 * \file  math.h
 * \brief Math functions
 *
 * math.h defines standard mathematical functions that are used by kernel.
 */

#ifndef _MATH_H
#define	_MATH_H

#include "types.h"

// internal functions, but external use allowed
/*!
 * \fn    int is_pow2(uint x)
 * \brief Checks if x is a power of 2.
 *
 * is_pow2 checks if x is a power of two performing ASM instructions bsf and bsr,
 * which return index of first and last high bit.
 *
 * \param  x unsigned integer to check
 * \return 0, if x isn't a power of 2, log2(x) if it it
 */
int is_pow2(uint x);

/*!
 * \fn    double pow_di(double base, int exponent)
 * \brief Raises base to exponent.
 *
 * pow_di raises double base to integer exponent, using iteration by squaring
 * method.
 *
 * \param  base     base of power
 * \param  exponent exponent of power
 * \return base^exponent
 */
double pow_di(double base, int exponent);
long double pow_ldi(long double base, int exponent);
double pow_ii(int base, int exponent);
int pow_iui(int base, uint exponent);

double pow(double base, double exponent);
double exp(double x);
double log(double x);
double fabs(double x);
double ceil(double x);
double floor(double x);
int trunc(float x);
double round(double x);
long int lround(double x);

#endif

