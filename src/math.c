#include "ale/math.h"

#define EPS (0.000001)

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
  return ale_gamma(a) * ale_gamma(b) / ale_gamma(a + b);
}


static double
ale_rilgamma_serie(double x, double a)
{
  double lgamma = ale_lgamma(a);
  double ai = a;
  double s = 1.0 / a;
  double delta = s;
  
  do
    {
      ai++;
      delta *=  x / ai;
      s += delta;
    }
  while (fabs(delta) >= fabs(s) * EPS);
    
  return s * exp(-x + a*log(x) - lgamma);
}

static double
ale_rilgamma_cont_frac(double x, double a)
{
  return 0.0;
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
