#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t n = 3, p = 3;
  double U[n][n], X[n][p], B[n][p], I[n][n];
  const double eps = 0.0000001;

  alg_identity_init(n, p, B);
  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = i ; j < n ; j++)
      U[i][j] = (i == j)?2:1;

  alg_UX_B_solve(n, p, U, B, X);
  print_m(n,n,X);
  alg_mul_m_m(n,n,n, U, X, I);
  print_m(n,n,I);
  

  /* for (size_t i = 0 ; i < m ; i++) */
  /*   for (size_t j = i+1 ; j < m ; j++) */
  /*   { */
  /*     double dot = alg_dot(m, Q[i], Q[j]); */
      
  /*     delta = fabs(0.0 - dot); */
  /*     ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: alg_QR_householder() 0.0 != C[%ld]*C[%ld] = %f\n", i, j, dot); */
  /*   } */



  return EXIT_SUCCESS;
}
