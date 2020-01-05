#ifndef __ALGEBRA_H
#define __ALGEBRA_H

#include <stddef.h>

int alg_Ux_b_solve(size_t n, const double U[n][n], const double b[n], double x[n]);
int alg_QR_Qtb_householder(size_t m, size_t n, size_t p, double A[m][n], double B[m][p]);

#endif
