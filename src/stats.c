#include <math.h>
#include <string.h>
#include <lapacke.h>
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
stats_diff_mean(size_t n, const double x[n], const double y[n])
{
  double s = 0;

  for (size_t i = 0 ; i < n ; i++)
    s += y[i] - x[i];

  return s / n;

}

double
stats_diff_var(size_t n, const double x[n], const double y[n])
{
  double m = stats_diff_mean(n, x, y);
  double s = 0;

  for (size_t i = 0 ; i < n ; i++)
    {
      double term = y[i] - x[i] - m;
      s += term * term;
    }

  return s / (n - 1);
}

double
stats_diff_sd(size_t n, const double x[n], const double y[n])
{
  return sqrt(stats_diff_var(n, x, y));
}
  
int
stats_pearson_corr(size_t n, const double x[n], const double y[n],
		   double *rho, double *pvalue)
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
  if ( sx <= 0 ||  sy <= 0)
    return -1;
  cov /= n - 1;
  sx = sqrt(sx / (n - 1));
  sy = sqrt(sy / (n - 1));

// Shieh, G. Behavior Research Methods (2010) 42: 906. https://doi.org/10.3758/BRM.42.4.906
  double r = cov / (sx * sy);
  double rs = 1-r*r;
  double rho2 = r * (1 + rs / (2 * (n - 2)) + 9 * rs * rs / (8 * n * (n - 2)));

  if (rho)
    *rho = rho2;
  if (pvalue)
    {
      if (1.0 == rho2)
	*pvalue = 0;
      else
	{
	  double t = rho2 * sqrt(n-2) / sqrt(1 - rho2*rho2);
	  *pvalue = 2 * (1 - stats_student_F(fabs(t), n-2));
	}
    }
  
  return 0;
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
stats_unif_std_rand()
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
  return min + (max - min) * stats_unif_std_rand();
}

double
stats_unif_std_F(double x)
{
  if (x <= 0)
    return 0;
  if (x >= 1)
    return 1;

  return x;
}

double
stats_unif_F(double x, double min, double max)
{
  if (x <= min)
    return 0;
  if (x >= max)
    return 1;

  return (x - min) / (max - min);
}

double
stats_norm_std_rand()
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
  return mu + sig * stats_norm_std_rand();
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
stats_student_rand(double df)
{
  // Num. recipes
  double u = stats_unif_rand(0,1);
  double v = stats_unif_rand(0,1);

  return sqrt(df * ( pow(u, -2/df) - 1 ) ) * cos(2*M_PI*v);
}

double
stats_student_F(double x, double df)
{
  if (x < 0)
    return 1 - stats_student_F(-x, df);

  return 1 - 0.5 * ale_ibeta(df / (df + x*x), 0.5*df, 0.5);
}
  
double
stats_gamma_rand_k_theta(double k, double theta)
{
  // A Simple Method for Generating Gamma -- GEORGE MARSAGLIA, WAI WAN TSANG
  double u, x, v;
  double d = k - 1.0 / 3.0;
  double c = (1.0 / 3.0) / sqrt (d);

  if (k < 1)
    {
      u = stats_unif_std_rand();
      return stats_gamma_rand(1.0 + k, theta) * pow(u, 1.0 / k);
    }
  
  do
    {
      do
	{
	  x = stats_norm_std_rand();
	  v = 1.0 + c * x;
	}
      while (v <= 0);
      
      v = v * v * v;
      u = stats_unif_std_rand();
    }
  while (u >= 1 - 0.0331 * x * x * x * x &&
	 log(u) >= 0.5 * x * x + d * (1 - v + log (v)));
  
  return theta * d * v;  
}

double
stats_gamma_rand(double alpha, double beta)
{
  return stats_gamma_rand_k_theta(alpha, 1/beta);
}

double
stats_gamma_F(double x, double alpha, double beta)
{
  return ale_rilgamma(beta * x, alpha);
}


// https://en.wikipedia.org/wiki/Beta_distribution#Generating_beta-distributed_random_variates
double
stats_beta_rand(double alpha, double beta)
{
  double x = stats_gamma_rand(alpha, 1);
  double y = stats_gamma_rand(beta, 1);
  
  return x / (x + y) ;
}

double
stats_beta_F(double x, double alpha, double beta)
{
  return ale_ibeta(x, alpha, beta);
}

void
stats_beta_fit_mm(size_t n, const double x[n], double *alpha, double *beta)
{
  double m = stats_mean(n, x);
  double v = stats_var(n, x);
  double t1_m = 1-m;
  double tfrac = m * t1_m / v - 1;
  
  *alpha = m*tfrac;
  *beta =  t1_m *tfrac;
}

static double
H0_student_pvalue(int H0, double t, double df)
{
  if (0 == H0)
    return 2 * (1-stats_student_F(fabs(t), df)); // two-sided
  else if (0 < H0)
    return 1 - stats_student_F(t, df); // greater
  
  return stats_student_F(t, df); // smaller
}

static inline double
t_test(size_t n, double m, double s, double mu, int H0,
       struct stats_t_test *data)
{
  double t, pvalue;

  if (s == 0)
    return -1;
  
  t = (m - mu) / (s / sqrt(n));
  pvalue = H0_student_pvalue(H0, t, n-1);
  
  if (data)
    {
      data->pvalue = pvalue;
      data->t = t;
      data->df = n-1;
    }
  
  return pvalue;
}
  
double
stats_t_test(size_t n, const double x[n], double mu, int H0,
	     struct stats_t_test *data)
{
  double m = stats_mean(n, x);
  double s = stats_sd(n, x);
  
  return t_test(n, m, s, mu, H0, data);
}


double
stats_t_test_paired(size_t n, const double x[n], const double y[n],
		    double mu, int H0,
		    struct stats_t_test *data)
{
  double m = stats_diff_mean(n, x, y);
  double s = stats_diff_sd(n, x, y);

  return t_test(n, m, s, mu, H0, data);
}

double
stats_t_test_welch(size_t nx, const double x[nx], size_t ny, const double y[ny],
		    double mu, int H0,
		    struct stats_t_test *data)
{
  // https://en.wikipedia.org/wiki/Welch%27s_t-test
  double t, pvalue;
  double mx = stats_mean(nx, x);
  double my = stats_mean(ny, y);
  double sx = stats_sd(nx, x);
  double sy = stats_sd(ny, y);
  if (sx+sy == 0)
    return -1;
  double num = sx*sx/nx + sy*sy/ny;
  double df = num*num / (sx*sx*sx*sx / (nx*nx * (nx-1)) + sy*sy*sy*sy / (ny*ny * (ny-1)) );   
  t = (my - mx - mu) / sqrt(num);
  pvalue = H0_student_pvalue(H0, t, df);
  
  if (data)
    {
      data->pvalue = pvalue;
      data->t = t;
      data->df = df;
    }
  
  return pvalue;
}

double
stats_rss(size_t m, size_t n, const double y[m], const double x[m][n],
	  double (*predict)(const double x[n], void *cls), void *cls)
{
  double rss = 0;
  for (size_t i = 0 ; i < m ; i++)
    {
      double term = y[i] - predict(x[i], cls);
      rss += term*term;
    }

  return rss;
}

int
stats_rsquared(size_t m, size_t n, const double y[m], const double x[m][n],
	       double (*predict)(const double x[n], void *cls), void *cls, double *rsquared)
{
  double rss = 0, ssy = 0;
  double my = stats_mean(m, y);
  for (size_t i = 0 ; i < m ; i++)
    {
      double term = y[i] - predict(x[i], cls);
      double ssy_term = my - y[i];
      rss += term*term;
      ssy += ssy_term*ssy_term;
    }

  if (0.0 == ssy)
    return -1;

  // https://en.wikipedia.org/wiki/Residual_sum_of_squares#Relation_with_Pearson's_product-moment_correlation

  *rsquared = 1 - rss/ssy;
  
  return 0;
}

int
stats_lm(size_t m, size_t n, size_t k, const double y[m][k], const double x[m][n],
	 double beta[n+1][k], double rss[k])
{
  double A[m][n+1]; // = [1|x];
  double B[m][k]; // = y

  memcpy(B, y, sizeof(double) * m * k);
  for (size_t i = 0 ; i < m ; i++)
    {
      A[i][0] = 1;
      for (size_t j = 0 ; j < n ; j++)
	 A[i][j+1] = x[i][j];
    }
      
  int info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', m, n+1, k,
			   (double*) A, n+1, (double*) B, k);


  for (size_t j = 0 ; j < k ; j++)
    rss[j] = 0;
  if (m >= n+1)
    {
      for (size_t i = 0 ; i < n+1 ; i++)
	for (size_t j = 0 ; j < k ; j++)
	  beta[i][j] = B[i][j];
      for (size_t i = n+1 ; i < m ; i++)
	for (size_t j = 0 ; j < k ; j++)
	  rss[j] += B[i][j] * B[i][j];      
    }
  
  return info;
}
