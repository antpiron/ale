#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"


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

int
main(int argc, char *argv[argc])
{
  const size_t m = 3, n = 3, p = 3;
  double A[3][3] = { {1, 1, 1},
		     {1, 2, 2},
		     {1, 2, 3}} ;
  double B[3][3] = { {1, 0, 0},
		     {0, 1, 0},
		     {0, 0, 1}};
  double C[3][3];

  alg_QR_Qtb_householder(m, n, p, A, B);
  print_m(m, n, A);
  printf("\n");
  print_m(m, n, B);

  alg_mul_m_m(m,n,p, B, A, C);
  printf("\n");
  print_m(m, n, C);


  // res = ale_beta(1, 1);
  // delta = fabs(res - 1);
  // ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(1,1) == %f != 1\n",
  //		res);


  return EXIT_SUCCESS;
}
