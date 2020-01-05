#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"

int
main(int argc, char *argv[argc])
{
  const size_t m = 2, n = 2, p = 2;
  double A[2][2] = { {1, 1},
		     {1, 2}} ;
  double B[2][2] = { {1, 0},
		     {0, 1}};

  alg_QR_Qtb_householder(m, n, p, A, B);
  printf("%f %f\n", A[0][0], A[0][1]);
  printf("%f %f\n", A[1][0], A[1][1]);
  printf("\n");
  printf("%f %f\n", B[0][0], B[0][1]);
  printf("%f %f\n", B[1][0], B[1][1]);
  // res = ale_beta(1, 1);
  // delta = fabs(res - 1);
  // ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(1,1) == %f != 1\n",
  //		res);


  return EXIT_SUCCESS;
}
