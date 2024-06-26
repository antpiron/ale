#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t n = 3, p = 3;
  double L[n][n], X[n][p], B[n][p], I[n][n];
  const double eps = 0.0000001;

  alg_identity_init(n, p, B);
  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0 ; j < n ; j++)
      if (j < i + 1)
	L[i][j] = (i == j)?2:1;
      else
	L[i][j] = 0;

  printf("L=\n");
  print_m(n,n,L);
  alg_LX_B_solve(n, p, L, B, X);
  printf("X=\n");
  print_m(n,p,X);
  alg_mul_m_m(n,n,n, L, X, I);
  printf("LX=\n");
  print_m(n,n,I);
  

  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0; j < n ; j++)
      {
	double delta = fabs(((i == j)?1:0) - I[i][j]);
	ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: alg_LX_B_solve() delta[%ld, %ld] == %f != 0", i, j, delta);
      }

  return EXIT_SUCCESS;
}
