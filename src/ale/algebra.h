#ifndef __ALGEBRA_H
#define __ALGEBRA_H

#include <stddef.h>

int alg_Ux_b_solve(size_t n, const double U[n][n], const double b[n], double x[n]);
int alg_qr_hh(size_t m, size_t n, const double A[m][n], double Q[n][n], double R[n][n]);

#endif
