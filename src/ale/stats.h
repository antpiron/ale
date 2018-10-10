#ifndef __STATS_H
#define __STATS_H

#include <stddef.h>

double stats_mean(size_t n, const double x[n]);
double stats_var(size_t n, const double x[n]);
double stats_cov(size_t n, const double x[n], const double y[n]);

#endif
