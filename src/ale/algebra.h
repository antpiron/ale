#ifndef __ALGEBRA_H
#define __ALGEBRA_H

#include <stddef.h>

#define ALG_GENERIC_HEADERS(SUFFIX,TYPE)				\
  TYPE* alg_add_v_v##SUFFIX(size_t n, TYPE v1[n], TYPE v2[n], TYPE res[n]); \
  TYPE* alg_sub_v_v##SUFFIX(size_t n, TYPE v1[n], TYPE v2[n], TYPE res[n]); \
  TYPE* alg_mul_v_c##SUFFIX(size_t n, const TYPE vec[n], TYPE d,  TYPE res[n]); \
  TYPE* alg_mul_m_v##SUFFIX(size_t m, size_t n, const TYPE A[m][n], const TYPE vec[n], TYPE res[n]); \
  TYPE*	alg_div_v_c##SUFFIX(size_t n, const TYPE vec[n], TYPE d,  TYPE res[n]); \
  TYPE* alg_opposite_v##SUFFIX(size_t n, TYPE v[n], TYPE res[n]);	\
  TYPE* alg_mul_m_v##SUFFIX(size_t m, size_t n, const TYPE A[m][n], const TYPE vec[n], TYPE res[n]); \
  									\
  TYPE* alg_identity_init##SUFFIX(size_t m, size_t n, TYPE A[m][n]);	\
  TYPE* alg_transpose##SUFFIX(size_t m, size_t n, const TYPE A[m][n], TYPE res[n][m]); \
  TYPE* alg_mul_m_m##SUFFIX(size_t m, size_t n, size_t p, const TYPE A[m][n], const TYPE B[n][p], TYPE res[m][p]); \
  int alg_UX_B_solve##SUFFIX(size_t n, size_t p, const TYPE U[n][n], const TYPE B[n][p], TYPE X[n][p]); \
  int alg_LX_B_solve##SUFFIX(size_t n, size_t p, const TYPE L[n][n], const TYPE B[n][p], TYPE X[n][p]); \
									\
  int alg_U_inverse##SUFFIX(size_t n, const TYPE U[n][n], TYPE X[n][n]); \
  int alg_L_inverse##SUFFIX(size_t n, const TYPE L[n][n], TYPE X[n][n]); \
									\
  int alg_AX_B_solve##SUFFIX(size_t n, size_t p, TYPE A[n][n], TYPE B[n][p], TYPE X[n][p]); \
  int alg_AX_B_OLS_solve##SUFFIX(size_t m, size_t n, size_t p, TYPE A[m][n], TYPE B[m][p], TYPE X[n][p]); \
  									\
  TYPE alg_norm##SUFFIX(size_t n, const TYPE vec[n]);			\
  TYPE alg_dot##SUFFIX(size_t n, const TYPE v1[n], const TYPE v2[n]);	\
  									\
  void print_m##SUFFIX(size_t m, size_t n, TYPE A[m][n]);		\
  void print_v##SUFFIX(size_t n, TYPE v[n]);				\
  									\
  int alg_QR_householder##SUFFIX(size_t m, size_t n, TYPE A[m][n], TYPE V[n][m]); \
  int householder_proj_QtB##SUFFIX(size_t m, size_t n, size_t p, TYPE V[n][m], TYPE B[m][p]); \
  int householder_proj_QX##SUFFIX(size_t m, size_t n, size_t p, TYPE V[n][m], TYPE X[m][p]);

ALG_GENERIC_HEADERS(,double)
ALG_GENERIC_HEADERS(l,long double)


#endif
