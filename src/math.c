#include <stddef.h>
#include "ale/math.h"


// Regularized Incomplete Lower gamma function
// IG(x;a) in http://www.stat.tamu.edu/~jnewton/604/chap3.pdf
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
  while (fabs(delta) >= fabs(sum) * ALE_EPS);
    
  return sum * exp(-x + a*log(x) - lgamma);
}

/* static double */
/* ale_rilgamma_cont_frac(double x, double a) */
/* { */
/*   double b = x + 1.0 - a; */
/*   double c = 1.0 / ALE_TINY_DOUBLE; */
/*   double d = 1.0 / b; */
/*   double f = d; */
/*   double delta; */
/*   double lgamma = ale_lgamma(a); */

/*   size_t i = 1; */
/*   do */
/*     { */
/*       double ai = -i * (i - a); */
/*       b += 2.0; */
/*       d = ai * d + b; */
/*       if (fabs(d) < ALE_TINY_DOUBLE) */
/* 	d = ALE_TINY_DOUBLE; */
      
/*       c = b + ai/c; */
/*       if (fabs(c) < ALE_TINY_DOUBLE) */
/* 	c = ALE_TINY_DOUBLE; */
      
/*       d = 1.0/d; */
/*       delta = d*c; */
/*       f *= delta; */
/*       i++; */
/*     } */
/*   while (fabs(delta - 1.0) > ALE_EPS); */
  
/*   return 1 - exp(-x + a*log(x) - lgamma) * f; */
/* } */

double
ale_rilgamma(double x, double a)
{
  if (0.0 == x)
    return 0.0;

  //if (x <= a + 1)          
  return ale_rilgamma_serie(x, a);

  // For some reason do not return the same value than R
  //return ale_rilgamma_cont_frac(x, a);
}

double
ale_riugamma(double x, double a)
{
  return 1 - ale_rilgamma(x, a);
}

double
ale_ibeta(double x, double a, double b)
{
  // 0 <= x <= 1, a,b > 0
  // http://www.stat.tamu.edu/~jnewton/604/chap3.pdf
  if (x == 0.0 || x == 1.0)
    return x;

  if ( x > (a+1.0) / (a+b+2.0) ) 
    return 1.0 - ale_ibeta(1.0-x, b, a);

  
  double factor = exp( log(x)*a + log(1.0-x)*b - ale_lbeta(a,b) ) / a;
  double f = 1.0, c = 1.0, d = 0.0;

  double frac_cont = 1.0;
  size_t m = 0, i = 0;

  while(1)
    {
      d = 1.0 + frac_cont * d;
      if (fabs(d) < ALE_TINY_DOUBLE)
	d = ALE_TINY_DOUBLE;
      d = 1.0 / d;
      
      c = 1.0 + frac_cont / c;
      if (fabs(c) < ALE_TINY_DOUBLE)
	c = ALE_TINY_DOUBLE;
       
      double delta = c*d;
      f *= delta;

      if (fabs(1.0 - delta) < ALE_EPS)
	break;
      
      if (++i >= 5000)
	break;
      
      m = i/2;
      frac_cont = (i % 2)?-((a+m)*(a+b+m)*x)/((a+2.0*m)*(a+2.0*m+1)):
	(m*(b-m)*x)/((a+2.0*m-1.0)*(a+2.0*m));
    }
  
  return factor * (f - 1.0);
}
