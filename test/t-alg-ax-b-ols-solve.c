#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t n = 3;
  double A[n][n],  X[n][n], B[n][n];
  double A_copy[n][n], B_copy[n][n];
  double delta;
  int ret;
  const double eps = 0.0000001;

  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	A_copy[i][j] = A[i][j] = (i == j)?2:1;
      }
  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	double x = 0;
	for (size_t k = 0 ; k < n ; k++)
	  B_copy[i][j] = B[i][j] += A[i][k];
      }
  
  printf("\nA=\n");
  print_m(n, n, A);
  printf("\nB=\n");
  print_m(n, n, B);
  ret = alg_AX_B_OLS_solve(n, n, n, A, B, X);
  printf("\nA=\n");
  print_m(n, n, A);
  printf("\nB=\n");
  print_m(n, n, B);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_solve() ret = %d\n != 0", ret);
  
  printf("\nX=\n");
  print_m(n, n, X);

  alg_mul_m_m(n,n,n, A_copy, X, B);
  printf("\nB=\n");
  print_m(n, n, B);


  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0; j < n ; j++)
    {
      
      delta = fabs(B[i][j] - B_copy[i][j]);
      ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: alg_AX_B_solve() delta != C[%ld]*C[%ld] = %f\n", i, j, delta);
    }



  return EXIT_SUCCESS;
}
