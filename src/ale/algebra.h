#ifndef __ALGEBRA_H
#define __ALGEBRA_H

#include <stddef.h>

double* alg_sub_v_v(size_t n, double v1[n], double v2[n], double res[n]);

double* alg_identity_init(size_t m, size_t n, double A[m][n]);
double* alg_transpose(size_t m, size_t n, const double A[m][n], double res[n][m]);
double* alg_mul_m_m(size_t m, size_t n, size_t p, const double A[m][n], const double B[n][p], double res[m][p]);
double alg_norm(size_t n, const double vec[n]);
double alg_dot(size_t n, const double v1[n], const double v2[n]);

void print_m(size_t m, size_t n, double A[m][n]);

int alg_Ux_b_solve(size_t n, const double U[n][n], const double b[n], double x[n]);
int alg_QR_householder(size_t m, size_t n, double A[m][n], double V[n][m]);
int householder_proj_QtB(size_t m, size_t n, size_t p, double V[n][m], double B[m][p]);
int householder_proj_QX(size_t m, size_t n, size_t p, double V[n][m], double X[m][p]);

#endif
