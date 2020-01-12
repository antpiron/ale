#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t m = 5, n = 5, p = 5;
  double A[m][n], B[m][p], X[m][p], C[m][n];
  double exp_C[m][n];
  double res, delta;
  const double eps = 0.0000001;

  alg_identity_init(m, p, B);
  alg_identity_init(m, p, X);
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      exp_C[i][j] = A[i][j] = (i == j)?2:1;

  
  alg_QR_Qtb_householder(m, n, p, A, &B, &X);
  printf("\nA=\n");
  print_m(m, n, A);

  printf("B=\n");
  print_m(m, n, B);

  alg_transpose(m, n, B, B);
  printf("B^t=\n");
  print_m(m, n, B);

  printf("X=\n");
  print_m(m, p, X);

  alg_mul_m_m(m,n,p, X, A, C);
  printf("C=\n");
  print_m(m, n, C);

  res = 0;
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	double diff = exp_C[i][j] - C[i][j];
	res += diff *diff;
      }
  delta = sqrt(res) / (m * n);
  // res = alg_sub_v_v(m*m, (double[]) exp_C, (double[]) C,  
  // res = ale_alg(1, 1);
  // delta = fabs(res - 1);
  ERROR_UNDEF_FATAL(delta >= eps, "FAIL: alg_QR_Qtb_householder() exp_C != C \n");


  return EXIT_SUCCESS;
}
