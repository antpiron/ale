#include <stddef.h>
#include "ale/math.h"

#define EPS (1e-15)
#define TINY_DOUBLE (1e-30)

double
ale_gamma(double x)
{
  return tgamma(x);
}

double
ale_lgamma(double x)
{
  return lgamma(x);
}

double
ale_beta(double a, double b)
{
  return exp(ale_lgamma(a) + ale_lgamma(b) - ale_lgamma(a + b));
}


static double
ale_rilgamma_serie(double x, double a)
{
  double lgamma = ale_lgamma(a);
  double ai = a;
  double sum = 1.0 / a;
  double delta = sum;
  
  do
    {
      ai++;
      delta *=  x / ai;
      sum += delta;
    }
  while (fabs(delta) >= fabs(sum) * EPS);
    
  return sum * exp(-x + a*log(x) - lgamma);
}

static double
ale_rilgamma_cont_frac(double x, double a)
{
  double b = x + 1.0 - a;
  double c = 1.0 / TINY_DOUBLE;
  double d = 1.0 / b;
  double f = d;
  double delta;
  double lgamma = ale_lgamma(a);

  size_t i = 1;
  do
    {
      double ai = -i * (i - a);
      b += 2.0;
      d = ai * d + b;
      if (fabs(d) < TINY_DOUBLE)
	d = TINY_DOUBLE;
      
      c = b + ai/c;
      if (fabs(c) < TINY_DOUBLE)
	c = TINY_DOUBLE;
      
      d = 1.0/d;
      delta = d*c;
      f *= delta;
      i++;
    }
  while (fabs(delta - 1.0) > EPS);
  
  return exp(-x + a*log(x) - lgamma) * f;
}

double
ale_rilgamma(double x, double a)
{
  if (0.0 == x)
    return 1.0;

  if (x <= a + 1)          
    return ale_rilgamma_serie(x, a);

  return ale_rilgamma_cont_frac(x, a);
}

double
ale_riugamma(double x, double a)
{
  return 1 - ale_rilgamma(x, a);
}

double
ale_ibeta(double x, double a, double b)
{
  return 0;
}
