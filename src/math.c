#include "ale/math.h"

#define EPS (1e-30)
#define FPMIN (1e-15)

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
  int i;
  double b = x + 1.0 - a;
  double c = 1.0 / FPMIN;
  double d = 1.0 / b;
  double h = d;
  double an,delta;
  double lgamma = ale_lgamma(a);

  for (i=1 ; ; i++) {
    an = -i * (i - a);
    b += 2.0;
    d = an * d + b;
    if (fabs(d) < FPMIN)
      d = FPMIN;
    c = b + an/c;
    if (fabs(c) < FPMIN)
      c = FPMIN;
    d = 1.0/d;
    delta = d*c;
    h *= delta;
    if (fabs(delta - 1.0) <= EPS)
      break;
  }
  
  return exp(-x + a*log(x) - lgamma) * h;
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
