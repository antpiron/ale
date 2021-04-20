#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ale/algebra.h"
#include "ale/error.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  alg_sum_of_squares##SUFFIX(size_t n, const TYPE vec[n])		\
  {									\
    TYPE sum = 0;							\
    									\
    for (size_t i = 0 ; i < n ; i++)					\
      sum += vec[i] * vec[i];						\
    									\
    return sum;								\
  }									\
  									\
  TYPE									\
  alg_norm##SUFFIX(size_t n, const TYPE vec[n])				\
  {									\
    return sqrt##SUFFIX(alg_sum_of_squares##SUFFIX(n, vec));		\
  }									\
  									\
  TYPE*									\
  alg_normalize##SUFFIX(size_t n, const TYPE vec[n], TYPE res[n])	\
  {									\
    TYPE norm = alg_norm##SUFFIX(n, vec);				\
    									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = vec[i] / norm;						\
    									\
    return res;								\
  }									\
  									\
  TYPE*								\
  alg_identity_init##SUFFIX(size_t m, size_t n, TYPE A[m][n])		\
  {									\
    for (size_t i = 0 ; i < m ; i++)					\
      for (size_t j = 0 ; j < m ; j++)					\
	A[i][j] = ( i == j )?1:0;					\
    									\
    return &A[0][0];							\
  }									\
  									\
  TYPE									\
  alg_dot##SUFFIX(size_t n, const TYPE v1[n], const TYPE v2[n])		\
  {									\
    TYPE res = 0;							\
    for (size_t i = 0 ; i < n ; i++)					\
      res += v1[i] * v2[i];						\
    									\
    return res;								\
  }									\
  									\
  TYPE*									\
  alg_lin_comb2##SUFFIX(size_t n, TYPE a1, const TYPE v1[n],		\
			TYPE a2, const TYPE v2[n], TYPE res[n])		\
  {									\
    									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = a1*v1[i] + a2*v2[i];					\
    									\
    return res;								\
  }									\
  									\
  TYPE*									\
  alg_sub_v_v##SUFFIX(size_t n, TYPE v1[n], TYPE v2[n], TYPE res[n])	\
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = v1[i] - v2[i];						\
    									\
    return res;								\
  }									\
  									\
  TYPE*									\
  alg_add_v_v##SUFFIX(size_t n, TYPE v1[n], TYPE v2[n], TYPE res[n])	\
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = v1[i] + v2[i];						\
    									\
    return res;								\
  }									\
  									\
  TYPE*									\
  alg_opposite_v##SUFFIX(size_t n, TYPE v[n], TYPE res[n])		\
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = -v[i];							\
    									\
    return res;								\
  }									\
   									\
  TYPE*									\
  alg_div_v_c##SUFFIX(size_t n, const TYPE vec[n], TYPE d,  TYPE res[n]) \
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = vec[i] / d;						\
									\
    return res;								\
  }									\
									\
  TYPE*									\
  alg_mul_v_c##SUFFIX(size_t n, const TYPE vec[n], TYPE d,  TYPE res[n]) \
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = vec[i] * d;						\
									\
    return res;								\
  }									\
									\
  TYPE*									\
  alg_mul_m_v##SUFFIX(size_t m, size_t n, const TYPE A[m][n], const TYPE vec[n], TYPE res[n]) \
  {									\
    for (size_t i = 0 ; i < m ; i++)					\
      {									\
	res[i] = 0;							\
	for (size_t j = 0 ; j < n ; j++)				\
	  res[i] += A[i][j] * vec[j];					\
      }									\
									\
    return res;								\
  }									\
									\
  TYPE*									\
  alg_mul_vt_m##SUFFIX(size_t m, size_t n, const TYPE vec[m], const TYPE A[m][n], TYPE res[n]) \
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[n] = 0;							\
									\
    for (size_t i = 0 ; i < m ; i++)					\
      for (size_t j = 0 ; j < n ; j++)					\
	res[j] += A[i][j] * vec[i];					\
									\
    return res;								\
  }									\
									\
  TYPE*								\
  alg_mul_m_m##SUFFIX(size_t m, size_t n, size_t p, const TYPE A[m][n], const TYPE B[n][p], TYPE res[m][p]) \
  {									\
    for (size_t i = 0 ; i < m ; i++)					\
      for (size_t j = 0 ; j < p ; j++)					\
	{								\
	  res[i][j] = 0;						\
	  for (size_t k = 0 ; k < n ; k++)				\
	    res[i][j] += A[i][k] * B[k][j];				\
	}								\
									\
    return res[0];							\
  }									\
									\
  TYPE*								\
  alg_mul_L_A##SUFFIX(size_t n, size_t p, const TYPE L[n][n], const TYPE A[n][p], TYPE LA[n][p]) \
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      for (size_t j = 0 ; j < p ; j++)					\
	{								\
	  LA[i][j] = 0;							\
	  for (size_t k = 0 ; k < i+1 ; k++)				\
	    LA[i][j] += L[i][k] * A[k][j];				\
	}								\
									\
    return LA[0];							\
  }									\
									\
  TYPE*								\
  alg_transpose##SUFFIX(size_t m, size_t n, const TYPE A[m][n], TYPE res[n][m]) \
  {									\
    size_t min_m_n = (m < n)?m:n;					\
    if ( (void*) &res != (void*) &A )					\
      for (size_t i = 0 ; i < min_m_n ; i++)				\
	res[i][i] = A[i][i];						\
									\
    for (size_t i = 1 ; i < m ; i++)					\
      {									\
	size_t min_i_n = (i < n)?i:n;					\
	for (size_t j = 0 ; j < min_i_n ; j++)				\
	  {								\
	    TYPE tmp = A[i][j];					\
	    res[i][j] = A[j][i];					\
	    res[j][i] = tmp;						\
	  }								\
      }									\
									\
    return (TYPE*) res;						\
  }									\
									\
  TYPE*								\
  alg_U_transpose##SUFFIX(size_t n, const TYPE U[n][n], TYPE L[n][n])	\
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	L[i][i] = U[i][i];						\
	for (size_t j = i+1 ; j < n ; j++)				\
	  {								\
	    L[j][i] = U[i][j];						\
	    L[i][j] = 0;						\
	  }								\
      }									\
									\
    return L[0];							\
  }									\
									\
  int									\
  alg_UX_B_solve##SUFFIX(size_t n, size_t p, const TYPE U[n][n], const TYPE B[n][p], TYPE X[n][p]) \
  {									\
    TYPE *sum = NULL;							\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      ERROR_CUSTOM_RET(0 == U[i][i], 1, -1);				\
									\
    sum = malloc(sizeof(TYPE) * p);					\
									\
    for (ssize_t i = n - 1 ; i >= 0 ; i--)				\
      {									\
	for (size_t k = 0 ; k < p ; k++)				\
	  sum[k] = 0;							\
	for (size_t j = i+1 ; j < n ; j++)				\
	  for (size_t k = 0 ; k < p ; k++)				\
	    sum[k] += U[i][j] * X[j][k];				\
									\
	for (size_t k = 0 ; k < p ; k++)				\
	  X[i][k] = (B[i][k] - sum[k]) / U[i][i] ;			\
      }									\
									\
    free(sum);								\
									\
    return 0;								\
  }									\
									\
  int									\
  alg_LX_B_solve##SUFFIX(size_t n, size_t p, const TYPE L[n][n], const TYPE B[n][p], TYPE X[n][p]) \
  {									\
    TYPE *sum = NULL;							\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      ERROR_CUSTOM_RET(0 == L[i][i], 1, -1);				\
									\
    sum = malloc(sizeof(TYPE) * p);					\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	for (size_t k = 0 ; k < p ; k++)				\
	  sum[k] = 0;							\
	for (size_t j = 0 ; j < i ; j++)				\
	  for (size_t k = 0 ; k < p ; k++)				\
	    sum[k] += L[i][j] * X[j][k];				\
									\
	for (size_t k = 0 ; k < p ; k++)				\
	  X[i][k] = (B[i][k] - sum[k]) / L[i][i] ;			\
      }									\
									\
    free(sum);								\
									\
    return 0;								\
  }									\
									\
									\
  /* destroy A and B */							\
  int									\
  alg_AX_B_solve##SUFFIX(size_t n, size_t p, TYPE A[n][n], TYPE B[n][p], TYPE X[n][p]) \
  {									\
    TYPE (*V)[n][n] = malloc(sizeof(*V));				\
    int ret;								\
									\
    alg_QR_householder##SUFFIX(n, n, A, *V);				\
    householder_proj_QtB##SUFFIX(n, n, p, *V, B);			\
									\
    ret = alg_UX_B_solve##SUFFIX(n, p, A, B, X);			\
									\
    free(V);								\
									\
    return ret;								\
  }									\
									\
  /* destroy A and B */							\
  int									\
  alg_AX_B_OLS_solve##SUFFIX(size_t m, size_t n, size_t p, TYPE A[m][n], TYPE B[m][p], TYPE X[n][p]) \
  {									\
    TYPE (*V)[n][m] = malloc(sizeof(*V));				\
    TYPE (*Rt)[n][n] = malloc(sizeof(*Rt));				\
    TYPE (*RtQtB)[n][p] = malloc(sizeof(*RtQtB));			\
    TYPE (*Y)[n][p] =  malloc(sizeof(*Y));				\
    int ret;								\
									\
    /* TODO: handle return code */					\
    alg_QR_householder##SUFFIX(m, n, A, *V);				\
    householder_proj_QtB##SUFFIX(m, n, p, *V, B);			\
    									\
    alg_U_transpose##SUFFIX(n, A, *Rt);					\
									\
    alg_mul_L_A##SUFFIX(n, p,  *Rt, B, *RtQtB);				\
    									\
    ret = alg_LX_B_solve##SUFFIX(n, p, *Rt, *RtQtB, *Y);		\
    if (ret >= 0)							\
      ret = alg_UX_B_solve##SUFFIX(n, p, A, *Y, X);			\
    									\
    									\
    free(V);								\
    free(Rt);								\
    free(RtQtB);							\
    free(Y);								\
									\
    return ret;								\
  }									\
									\
  int									\
  alg_QtR_mgs##SUFFIX(size_t m, size_t n, const TYPE A[m][n], TYPE Qt[n][m], TYPE R[n][n]) \
  {									\
    /* http://www.math.iit.edu/~fass/477577_Chapter_4.pdf */		\
    if (m < n)								\
      return -1;							\
									\
    alg_transpose##SUFFIX(m,n,A,Qt);					\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	R[i][i] = alg_norm##SUFFIX(m, Qt[i]);				\
	alg_div_v_c##SUFFIX(m, Qt[i], R[i][i], Qt[i]);			\
	for (size_t j = i+1 ; j < n ; j++)				\
	  {								\
	    R[i][j] = alg_dot##SUFFIX(m, Qt[i], Qt[i]);			\
	    alg_lin_comb2##SUFFIX(m, 1.0, Qt[j], -R[i][j], Qt[i], Qt[j]); \
	  }								\
      }									\
									\
    return 0;								\
  }									\
									\
  void									\
  householder_proj##SUFFIX(size_t m, size_t n, size_t k, TYPE v[k], TYPE A[m][n], TYPE vA[n]) \
  {									\
    /* dynamic alloc. or param */					\
    /* TYPE vA[n]; */							\
									\
    /* v^t * A_k:m,k:n */						\
    for (size_t i = 0 ; i < n ; i++)					\
      vA[i] = 0;							\
    for (size_t i = k ; i < m ; i++)					\
      for (size_t j = 0 ; j < n ; j++)					\
	vA[j] += v[i-k] * A[i][j];					\
									\
    /* A_k:m,k:n = A_k:m,k:n - 2 * v * v^t * A_k:m,k:n */		\
    for (size_t i = k ; i < m ; i++)					\
      for (size_t j = 0 ; j < n ; j++)					\
	A[i][j] += - 2 * v[i-k] * vA[j];				\
  }									\
									\
  int									\
  householder_proj_QtB##SUFFIX(size_t m, size_t n, size_t p, TYPE V[n][m], TYPE B[m][p]) \
  {									\
    TYPE (*vB)[p] = malloc(sizeof(*vB));				\
									\
    for (size_t k = 0 ; k < n ; k++)					\
      householder_proj##SUFFIX(m, p, k, V[k], B, *vB);				\
									\
    free(vB);								\
									\
    return 0;								\
  }									\
									\
  int									\
  householder_proj_QX##SUFFIX(size_t m, size_t n, size_t p, TYPE V[n][m], TYPE X[m][p]) \
  {									\
    TYPE (*vX)[p] = malloc(sizeof(*vX));				\
									\
    for (ssize_t k = n-1 ; k >= 0 ; k--)				\
      householder_proj##SUFFIX(m, p, k, V[k], X, *vX);				\
									\
    free(vX);								\
									\
    return 0;								\
  }									\
									\
									\
									\
  /* destroy A and B */							\
  int									\
  alg_QR_householder##SUFFIX(size_t m, size_t n, TYPE A[m][n], TYPE V[n][m]) \
  {									\
    int ret = 0;							\
    TYPE (*vA)[n];							\
									\
    if (m < n)								\
      return -1;							\
									\
    vA = malloc(sizeof(*vA));						\
    if ( NULL == vA)							\
      return -1;							\
									\
    /* https://math.dartmouth.edu/~m116w17/Householder.pdf */		\
									\
    for (size_t k = 0 ; k < n ; k++)					\
      {									\
	size_t mv = m - k;						\
	TYPE ss, norm;							\
									\
	for (size_t i = 0 ; i < mv ; i++)				\
	  V[k][i] = A[i+k][k];						\
	/* ||v_2:m-k|| */						\
	ss = alg_sum_of_squares##SUFFIX(mv-1, V[k]+1);			\
	/* ||v|| */							\
	norm = sqrt##SUFFIX(V[k][0]*V[k][0] + ss);			\
	/* v = v + sign(v1) * e1 */					\
	V[k][0] += copysign##SUFFIX(1.0, V[k][0]) * norm;		\
	/* ||v|| */							\
	norm = sqrt##SUFFIX(V[k][0]*V[k][0] + ss);			\
	if (0 == norm)							\
	  {								\
	    ret = -1;							\
	    break;							\
	  }								\
	alg_div_v_c##SUFFIX(mv, V[k], norm, V[k]);			\
									\
	/* A_k:m,k:n = A_k:m,k:n - 2 * v * v^t * A_k:m,k:n */		\
	householder_proj##SUFFIX(m, n, k, V[k], A, *vA);		\
      }									\
									\
    free(vA);								\
									\
    return ret;								\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)



void	
print_m(size_t m, size_t n, double A[m][n])
{
  for (size_t i = 0 ; i < m ; i++)
    {
      for (size_t j = 0 ; j < n ; j++)
	printf("%f\t", A[i][j]);
      printf("\n");
    }	
}

void	
print_ml(size_t m, size_t n, long double A[m][n])
{
  for (size_t i = 0 ; i < m ; i++)
    {
      for (size_t j = 0 ; j < n ; j++)
	printf("%Lf\t", A[i][j]);
      printf("\n");
    }	
}

void	
print_v(size_t n, double v[n])
{
  for (size_t i = 0 ; i < n ; i++)
    printf("%f\t", v[i]);
  printf("\n");
}

void	
print_vl(size_t n, long double v[n])
{
  for (size_t i = 0 ; i < n ; i++)
    printf("%Lf\t", v[i]);
  printf("\n");
}