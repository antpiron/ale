#include <math.h>
#include "ale/stats.h"

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
  double r = prod?0:cov / prod;
  double rs = 1-r*r;
  double rho = r * (1 + rs / (2 * (n - 2)) + 9 * rs * rs / (8 * n * (n - 2)));

  return rho;
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
	for (size_t k = i ; k < n ; k++)
	  s += (x[i][k] - means[i] ) * (x[j][k] - means[j]);
	cov[j][i] = cov[i][j] = s;
      }
}
