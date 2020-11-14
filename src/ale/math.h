#ifndef __ALE_MATH_H
#define __ALE_MATH_H

#include <math.h>

#define ALE_EPS (1E-15)
#define ALE_TINY_DOUBLE (1E-30)


static inline int ale_doublecmp(double a, double b, double eps);

static inline double ale_gamma(double x);
static inline double ale_lgamma(double x);
static inline long double ale_lgammal(long double x);
static inline double ale_beta(double a, double b);
static inline double ale_lbeta(double a, double b);

double ale_rilgamma(double x, double a);
double ale_riugamma(double x, double a);
double ale_ibeta(double x, double a, double b);

// Inline body

static inline
int
ale_doublecmp(double a, double b, double eps)
{
  double diff = a - b;
  double max = (fabs(a) >= fabs(b)) ? fabs(a) : fabs(b);
  
  if ( fabs(diff) <= max*eps )
    return 0;

  if ( diff < 0.0 )
    return -1;
  
  return 1;
}

static inline
double
ale_gamma(double x)
{
  return tgamma(x);
}

static inline
double
ale_lgamma(double x)
{
  return lgamma(x);
}

static inline
long double
ale_lgammal(long double x)
{
  return lgammal(x);
}

static inline
double
ale_beta(double a, double b)
{
  return exp(ale_lbeta(a, b));
}

static inline
double
ale_lbeta(double a, double b)
{
  return ale_lgamma(a) + ale_lgamma(b) - ale_lgamma(a + b);
}


#endif
