#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ale/algebra.h"


double
alg_sum_of_squares(size_t n, const double vec[n])
{
  double sum = 0;

  for (size_t i = 0 ; i < n ; i++)
    sum += vec[i] * vec[i];

  return sum;
}

double
alg_norm(size_t n, const double vec[n])
{
  return sqrt(alg_sum_of_squares(n, vec));
}

double*
alg_normalize(size_t n, const double vec[n], double res[n])
{
  double norm = alg_norm(n, vec);

  for (size_t i = 0 ; i < n ; i++)
    res[i] = vec[i] / norm;

  return res;
}

double*
alg_identity_init(size_t m, size_t n, double A[m][n])
{
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < m ; j++)
      A[i][j] = ( i == j )?1:0;

  return &A[0][0];
}

double
alg_dot(size_t n, const double v1[n], const double v2[n])
{
  double res = 0;
  for (size_t i = 0 ; i < n ; i++)
    res += v1[i] * v2[i];

  return res;
}

double*
alg_lin_comb2(size_t n, double a1, const double v1[n],
	      double a2, const double v2[n], double res[n])
{

  for (size_t i = 0 ; i < n ; i++)
    res[i] = a1*v1[i] + a2*v2[i];

  return res;
}

double*
alg_sub_v_v(size_t n, double v1[n], double v2[n], double res[n])
{
  for (size_t i = 0 ; i < n ; i++)
    res[i] = v1[i] - v2[i];
  
  return res;
}

double*
alg_div_v_c(size_t n, const double vec[n], double d,  double res[n])
{
  for (size_t i = 0 ; i < n ; i++)
    res[i] = vec[i] / d;

  return res;
}

double*
alg_mul_v_c(size_t n, const double vec[n], double d,  double res[n])
{
  for (size_t i = 0 ; i < n ; i++)
    res[i] = vec[i] * d;

  return res;
}

double*
alg_mul_m_v(size_t m, size_t n, const double A[m][n], const double vec[n], double res[n])
{
  for (size_t i = 0 ; i < m ; i++)
    {
      res[i] = 0;
      for (size_t j = 0 ; j < n ; j++)
	res[i] += A[i][j] * vec[j];
    }

  return res;
}

double*
alg_mul_vt_m(size_t m, size_t n, const double vec[m], const double A[m][n], double res[n])
{
  for (size_t i = 0 ; i < n ; i++)
    res[n] = 0;

  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      res[j] += A[i][j] * vec[i];

  return res;
}

double*
alg_mul_m_m(size_t m, size_t n, size_t p, const double A[m][n], const double B[n][p], double res[m][p])
{
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < p ; j++)
      {
	res[i][j] = 0;
	for (size_t k = 0 ; k < n ; k++)
	  res[i][j] += A[i][k] * B[k][j];
      }

  return res[0];
}

double*
alg_transpose(size_t m, size_t n, const double A[m][n], double res[n][m])
{
  size_t min_m_n = (m < n)?m:n;
  if ( (void*) &res != (void*) &A )
    for (size_t i = 0 ; i < min_m_n ; i++)
      res[i][i] = A[i][i];
  
  for (size_t i = 1 ; i < m ; i++)
    {
      size_t min_i_n = (i < n)?i:n;
      for (size_t j = 0 ; j < min_i_n ; j++)
	{
	  double tmp = A[i][j];
	  res[i][j] = A[j][i];
	  res[j][i] = tmp;
      }
    }

  return (double*) res;
}

int
alg_UX_B_solve(size_t n, size_t p, const double U[n][n], const double B[n][p], double X[n][p])
{
  double *sum = NULL;
	
  for (size_t i = 0 ; i < n ; i++)
    if (0 == U[i][i])
      return -1;

  sum = malloc(sizeof(double) * p);

  for (ssize_t i = n - 1 ; i >= 0 ; i--)
    { 
      for (size_t k = 0 ; k < p ; k++)
	sum[k] = 0;
      for (size_t j = i+1 ; j < n ; j++)
	for (size_t k = 0 ; k < p ; k++)
	  sum[k] += U[i][j] * X[j][k];
      
      for (size_t k = 0 ; k < p ; k++)
	X[i][k] = (B[i][k] - sum[k]) / U[i][i] ;
    }

  free(sum);

  return 0;
}

int
alg_LX_B_solve(size_t n, size_t p, const double L[n][n], const double B[n][p], double X[n][p])
{
  double *sum = NULL;
	
  for (size_t i = 0 ; i < n ; i++)
    if (0 == L[i][i])
      return -1;

  sum = malloc(sizeof(double) * p);

  for (size_t i = 0 ; i < n ; i++)
    { 
      for (size_t k = 0 ; k < p ; k++)
	sum[k] = 0;
      for (size_t j = 0 ; j < i ; j++)
	for (size_t k = 0 ; k < p ; k++)
	  sum[k] += L[i][j] * X[j][k];
      
      for (size_t k = 0 ; k < p ; k++)
	X[i][k] = (B[i][k] - sum[k]) / L[i][i] ;
    }

  free(sum);

  return 0;
}


int
alg_QtR_mgs(size_t m, size_t n, const double A[m][n], double Qt[n][m], double R[n][n])
{
  // http://www.math.iit.edu/~fass/477577_Chapter_4.pdf
  if (m < n)
    return -1;

  alg_transpose(m,n,A,Qt);
  for (size_t i = 0 ; i < n ; i++)
    {
      R[i][i] = alg_norm(m, Qt[i]);
      alg_div_v_c(m, Qt[i], R[i][i], Qt[i]);
      for (size_t j = i+1 ; j < n ; j++)
	{
	  R[i][j] = alg_dot(m, Qt[i], Qt[i]);
	  alg_lin_comb2(m, 1.0, Qt[j], -R[i][j], Qt[i], Qt[j]);
	}
    }

  return 0;
}


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
householder_proj(size_t m, size_t n, size_t k, double v[k], double A[m][n])
{
  // TODO: dynamic alloc. or param
  double vA[n];
  
  // v^t * A_k:m,k:n
  for (size_t i = 0 ; i < n ; i++)
    vA[i] = 0;
  for (size_t i = k ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      vA[j] += v[i-k] * A[i][j];
  
  // A_k:m,k:n = A_k:m,k:n - 2 * v * v^t * A_k:m,k:n
  for (size_t i = k ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      A[i][j] += - 2 * v[i-k] * vA[j];
}

int
householder_proj_QtB(size_t m, size_t n, size_t p, double V[n][m], double B[m][p])
{
  for (size_t k = 0 ; k < n ; k++)
    householder_proj(m, p, k, V[k], B);

  return 0;
}

int
householder_proj_QX(size_t m, size_t n, size_t p, double V[n][m], double X[m][p])
{
  for (ssize_t k = n-1 ; k >= 0 ; k--)
    householder_proj(m, p, k, V[k], X);
  
  return 0;
}



// destroy A and B
int
alg_QR_householder(size_t m, size_t n, double A[m][n], double V[n][m])
{
  if (m < n)
    return -1;
  
 // https://math.dartmouth.edu/~m116w17/Householder.pdf

  for (size_t k = 0 ; k < n ; k++)
    {
      size_t mv = m - k;
      double ss, norm;

      for (size_t i = 0 ; i < mv ; i++)
	V[k][i] = A[i+k][k];
      // ||v_2:m-k||
      ss = alg_sum_of_squares(mv-1, V[k]+1);
      // ||v||
      norm = sqrt(V[k][0]*V[k][0] + ss);
      // v = v + sign(v1) * e1
      V[k][0] += copysign(1.0, V[k][0]) * norm;
      // ||v||
      norm = sqrt(V[k][0]*V[k][0] + ss);
      if (0 == norm)
        return -1;
      alg_div_v_c(mv, V[k], norm, V[k]);

      // A_k:m,k:n = A_k:m,k:n - 2 * v * v^t * A_k:m,k:n
      householder_proj(m, n, k, V[k], A);
    }


  return 0;
}
