#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t m = 3, n = 3, p = 3;
  double A[3][3] = { {2, 1, 1},
		     {1, 2, 2},
		     {1, 2, 3}} ;
  double B[3][3] = { {1, 0, 0},
		     {0, 1, 0},
		     {0, 0, 1}};
  double C[3][3];

  alg_QR_Qtb_householder(m, n, p, A, B);
  printf("A=\n");
  print_m(m, n, A);
  printf("B=\n");
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
