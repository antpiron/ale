#ifndef __STATS_H
#define __STATS_H

#include <stddef.h>
#include <stdint.h>

double stats_mean(size_t n, const double x[n]);
double stats_var(size_t n, const double x[n]);
double stats_sd(size_t n, const double x[n]);
double stats_diff_mean(size_t n, const double x[n], const double y[n]);
double stats_diff_var(size_t n, const double x[n], const double y[n]);
double stats_diff_sd(size_t n, const double x[n], const double y[n]);
double stats_cov(size_t n, const double x[n], const double y[n]);
int stats_pearson_corr(size_t n, const double x[n], const double y[n],
		       double *rho, double *pvalue);
void stats_mat_cov(size_t m, size_t n, double cov[m][m], const double x[m][n]);

double stats_unif_std_rand();
double stats_unif_rand(double min, double max);
double stats_unif_std_F(double x);
double stats_unif_F(double x, double min, double max);

double stats_norm_std_rand();
double stats_norm_rand(double mu, double sig);
double stats_norm_std_F(double x);
double stats_norm_F(double x, double mu, double sig);

double stats_student_rand(double df);
double stats_student_F(double x, double df);

double stats_gamma_rand(double alpha, double beta);

struct stats_t_test {
  double pvalue, df, t;
};
double stats_t_test(size_t n, const double x[n], double mu, int H0,
		    struct stats_t_test *data);
double stats_t_test_paired(size_t n, const double x[n], const double y[n],
			   double mu, int H0,
			   struct stats_t_test *data);
double stats_t_test_welch(size_t nx, const double x[nx], size_t ny, const double y[ny],
			  double mu, int H0,
			  struct stats_t_test *data);

#endif
