#ifndef __FACTORIAL_H
#define __FACTORIAL_H

/*
 * This is a table of factorials of n < 21, which are used for example in
 * function exp (in which I decided to use Taylor's series). More info in
 * specification of math library.
 *
 * Implementing a function that would count factorials of n > 21 basing on
 * this table makes no sense, since it would require some 128 bits mathematics
 * (21! is biggest factorial which makes it in range of unsigned long long int).
 */

#define FACTORIALS 21

const unsigned long long int __factorial[] =
  { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600,
    6227020800ULL, 87178291200ULL, 1307674368000ULL, 20922789888000ULL,
    355687428096000ULL, 6402373705728000ULL, 121645100408832000ULL, 2432902008176640000ULL };
#endif