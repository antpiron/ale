#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t m = 2, n = 2, p = 2;
  double A[m][n], B[m][n], C[m][n];

  alg_identity_init(m, n, B);
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      A[i][j] = (i == j)?2:1;
  
  alg_QR_Qtb_householder(m, n, p, A, B);
  printf("\nA=\n");
  print_m(m, n, A);

  printf("B=\n");
  print_m(m, n, B);
  alg_transpose(m, n, B, B);
  printf("B^t=\n");
  print_m(m, n, B);

  alg_mul_m_m(m,n,p, B, A, C);
  printf("C=\n");
  print_m(m, n, C);


  

  // res = ale_beta(1, 1);
  // delta = fabs(res - 1);
  // ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(1,1) == %f != 1\n",
  //		res);


  return EXIT_SUCCESS;
}
