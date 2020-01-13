#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t m = 4, n = 3;
  double A[m][n],  V[n][m], C[m][n];
  double Qt[m][m], Q[m][m];
  double exp_C[m][n];
  double res, delta;
  const double eps = 0.0000001;

  alg_identity_init(m, m, Q);
  alg_identity_init(m, m, Qt);
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      exp_C[i][j] = A[i][j] = (i == j)?2:1;

  
  alg_QR_householder(m, n, A, V);
  householder_proj_QtB(m, n, m, V, Qt);
  householder_proj_QX(m, n, m, V, Q);
  printf("\nA=\n");
  print_m(m, n, A);

  printf("Qt=\n");
  print_m(m, m, Qt);

  alg_transpose(m, m, Qt, Qt);
  printf("Qt^t=\n");
  print_m(m, m, Qt);

  printf("Q=\n");
  print_m(m, m, Q);

  alg_mul_m_m(m,m,n, Q, A, C);
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
  ERROR_UNDEF_FATAL(delta >= eps, "FAIL: alg_QR_Qtb_householder() exp_C != C = QR\n");


  return EXIT_SUCCESS;
}
