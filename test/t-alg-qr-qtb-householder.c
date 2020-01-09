#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t m = 3, n = 3, p = 3;
  double A[m][n], B[m][p], X[m][p], C[m][n];

  alg_identity_init(m, p, B);
  alg_identity_init(m, p, X);
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      A[i][j] = (i == j)?2:1;
  
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

  

  // res = ale_beta(1, 1);
  // delta = fabs(res - 1);
  // ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(1,1) == %f != 1\n",
  //		res);


  return EXIT_SUCCESS;
}
