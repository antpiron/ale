#include <math.h>
#include "ale/stats.h"
#include "ale/error.h"
#include "ale/portability.h"
#include "ale/math.h"

double
stats_mean(size_t n, const double x[n])
{
  double s = 0;

  for (size_t i = 0 ; i < n ; i++)
    s += x[i];

  return s / n;
}

double
stats_var(size_t n, const double x[n])
{
  double m = stats_mean(n, x);
  double s = 0;

  for (size_t i = 0 ; i < n ; i++)
    {
      double term = x[i] - m;
      s += term * term;
    }

  return s / (n - 1);
}

double
stats_sd(size_t n, const double x[n])
{
  return sqrt(stats_var(n, x));
}

double
stats_cov(size_t n, const double x[n], const double y[n])
{
  double mx = stats_mean(n, x);
  double my = stats_mean(n, y);
  double s = 0;

  for (size_t i = 0 ; i < n ; i++)
    s += (x[i] - mx) * (y[i] - my);

  return s / (n - 1);
}

double
stats_pearson_corr(size_t n, const double x[n], const double y[n])
{
  double mx = stats_mean(n, x);
  double my = stats_mean(n, y);
  double cov = 0, sx = 0, sy = 0;

  for (size_t i = 0 ; i < n ; i++)
    {
      double xterm = x[i] - mx;
      double yterm = y[i] - my;
      cov += xterm * yterm;
      sx += xterm * xterm;
      sy += yterm * yterm;
    }
  cov /= n - 1;
  sx = sqrt(sx / (n - 1));
  sy = sqrt(sy / (n - 1));

// Shieh, G. Behavior Research Methods (2010) 42: 906. https://doi.org/10.3758/BRM.42.4.906
  double prod = sx * sy;
  double r = prod?cov / prod:0;
  double rs = 1-r*r;
  double rho = r * (1 + rs / (2 * (n - 2)) + 9 * rs * rs / (8 * n * (n - 2)));

  return rho;
}

double
stats_pearson_corr_full(size_t n, const double x[n], const double y[n], double *pvalue)
{
  // https://support.minitab.com/en-us/minitab-express/1/help-and-how-to/modeling-statistics/regression/how-to/correlation/methods-and-formulas/
  double r = stats_pearson_corr(n, x, y);
  double t = fabs(r) * sqrt(n-2) / sqrt(1 - r*r);

  *pvalue = 2 * (1 - stats_student_std_F(t, n-2));

  return r;
}


void
stats_mat_cov(size_t m, size_t n, double cov[m][m], const double x[m][n])
{
  double means[m];

  for (size_t i = 0 ; i < m ; i++)
    means[i] = stats_mean(n, x[i]);

  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = i ; j < m ; j++)
      {
	double s = 0;
	for (size_t k = 0 ; k < n ; k++)
	  s += (x[i][k] - means[i] ) * (x[j][k] - means[j]);
	cov[j][i] = cov[i][j] = s / (n - 1);
      }
}

double
stats_unif_rand_std()
{
  // generate (0,1) and not [0,1]
  uint64_t a;
  double r;

  do
    {
      ERROR_ERRNO_MSG(-1 == getrandom(&a, sizeof(a), 0),
		      "stats_unif_rand_std(): failed");
      r = (double) a / (double) UINT64_MAX;
    }
  while ( r <= 0.0 || r >= 1.0 );
  
  return r;
}
  
double
stats_unif_rand(double min, double max)
{  
  return min + (max - min) * stats_unif_rand_std();
}
double
stats_unif_rand_std_F(double x)
{
  if (x <= 0)
    return 0;
  if (x >= 1)
    return 1;

  return x;
}

double
stats_unif_rand_F(double x, double min, double max)
{
  if (x <= min)
    return 0;
  if (x >= max)
    return 1;

  return (x - min) / max;
}

double
stats_norm_rand_std()
{
  // https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
  static int flag = 1;
  static double val;
  double ret;
  
  if (flag)
    {
      double s,u,v;
      do
	{
	  u = stats_unif_rand(-1,1);
	  v = stats_unif_rand(-1,1);
	  s = u*u+v*v;
	}
      while (s >= 1.0 || s == 0.0);
      
      double root = sqrt(-2.0*log(s)/s); 
      val = u*root;
      ret = v*root;
      flag = 0;
    }
  else
    {
      flag = 1;
      ret = val;
    }

  return ret;
}

double
stats_norm_rand(double mu, double sig)
{
  return mu + sig * stats_norm_rand_std();
}

double
stats_norm_std_F(double x)
{
  // http://www.stat.tamu.edu/~jnewton/604/chap3.pdf
  if (x < 0)
    return 1 - stats_norm_std_F(-x);
  
  return 0.5 + 0.5 * ale_rilgamma(x*x*0.5, 0.5);
}

double
stats_norm_F(double x, double mu, double sig)
{
  return stats_norm_std_F( (x - mu) / sig );
}

double
stats_student_rand_std(uint64_t n)
{
  // Num. recipes
  double u = stats_unif_rand(0,1);
  double v = stats_unif_rand(0,1);

  return sqrt(n * ( pow(u, -2/(double)n) - 1 ) ) * cos(2*M_PI*v);
}

double
stats_student_std_F(double x, uint64_t n)
{
  if (x < 0)
    return 1 - stats_student_std_F(-x, n);

  return 1 - 0.5 * ale_ibeta(n / (n + x*x), 0.5*n, 0.5);
}
  
double
stats_student_rand(uint64_t n, double mu, double sig)
{
  return mu + sig * stats_student_rand_std(n);
}

double
stats_gamma_rand(double alpha, double beta)
{
  // A Simple Method for Generating Gamma -- GEORGE MARSAGLIA, WAI WAN TSANG
  double u, x, v;
  double d = alpha - 1.0 / 3.0;
  double c = (1.0 / 3.0) / sqrt (d);

  if (alpha < 1)
    {
      u = stats_unif_rand_std();
      return stats_gamma_rand(1.0 + alpha, beta) * pow(u, 1.0 / alpha);
    }
  
  do
    {
      do
	{
	  x = stats_norm_rand_std();
	  v = 1.0 + c * x;
	}
      while (v <= 0);
      
      v = v * v * v;
      u = stats_unif_rand_std();
    }
  while (u >= 1 - 0.0331 * x * x * x * x &&
	 log(u) >= 0.5 * x * x + d * (1 - v + log (v)));
  
  return beta * d * v;  
}
