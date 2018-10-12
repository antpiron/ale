#ifndef __STATS_H
#define __STATS_H

#include <stddef.h>

double stats_mean(size_t n, const double x[n]);
double stats_var(size_t n, const double x[n]);
double stats_sd(size_t n, const double x[n]);
double stats_cov(size_t n, const double x[n], const double y[n]);
double stats_pearson_corr(size_t n, const double x[n], const double y[n]);
void stats_mat_cov(size_t m, size_t n, double cov[m][m], const double x[m][n]);

double stats_unif_rand_std();
double stats_unif_rand(double min, double max);

double stats_norm_rand_std();
double stats_norm_rand(double mu, double sig);

#endif
