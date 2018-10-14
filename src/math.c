#include "ale/math.h"

double
ale_gamma(double x)
{
  return tgamma(x);
}

double
ale_beta(double a, double b)
{
  return ale_gamma(a) * ale_gamma(b) / ale_gamma(a + b);
}

double
ale_ilgamma(double x, double a)
{
  return 0;
}

double
ale_iugamma(double x, double a)
{
  return 1 - ale_ilgamma(x, a);
}

double
ale_ibeta(double x, double a, double b)
{
  return 0;
}
