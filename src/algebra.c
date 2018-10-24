#include <string.h>
#include <unistd.h>
#include <math.h>
#include "ale/algebra.h"


double
alg_norm(size_t n, const double vec[n])
{
  double sum = 0;

  for (size_t i = 0 ; i < n ; i++)
    sum += vec[i] * vec[i];

  return sqrt(sum);
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

double*
alg_div_v_c(size_t n, double vec[n], double d,  double res[n])
{
  for (size_t i = 0 ; i < n ; i++)
    res[i] = vec[i] / d;

  return res;
}

double*
alg_mul_v_c(size_t n, double vec[n], double d,  double res[n])
{
  for (size_t i = 0 ; i < n ; i++)
    res[i] = vec[i] * d;

  return res;
}

int
alg_Ux_b_solve(size_t n, const double U[n][n], const double b[n], double x[n])
{

  for (ssize_t i = n - 1 ; i >= 0 ; i--)
    {
      double sum = b[i];
      for (size_t j = i+1 ; j < n ; j++)
	sum -= U[i][j] * x[j];

      if (0.0 == U[i][i])
	return -1;

      x[i] = sum / U[i][i] ;
    }
  
  return 0;
}



int
alg_qr_mgs(size_t m, size_t n, const double A[m][n], double Q[m][n], double R[m][n])
{
  // http://www.math.iit.edu/~fass/477577_Chapter_4.pdf
  
}


int
alg_qr_hh(size_t m, size_t n, const double A[m][n], double Q[m][n], double R[m][n])
{
  if (m < n)
    return -1;

  alg_identity_init(m, n, Q);
  memcpy(R, A, n*m*sizeof(double));

  for (size_t j = 0 ; j < n ; j++)
    {
      size_t nw = n-j;
      double w[nw];
      double s, u1, tau, norm;
      
      for (size_t i = 0 ; i < nw ; i++)
	w[i] = R[i+j][j];
      norm = alg_norm(nw, w);

      // w[0] ==  R[j][j]
      s =  - copysign(1.0, w[0]);
      u1 = w[0] - s * norm;
      alg_div_v_c(nw, w, u1, w);
      w[0] = 1;
      tau = -s * u1 / norm;

      double (*RR)[n] = &R[j];
      // alg_sub_m(nw, n, RR, );
    }
  
  return 0;
}

