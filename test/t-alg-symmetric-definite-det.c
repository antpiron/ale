#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/math.h"


int
main(int argc, char *argv[argc])
{
  const size_t n = 3;
  double A[n][n];
  double det, exp;
  const double eps = 0.0000001;

  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = i ; j < n ; j++)
      A[i][j] = (i == j)?2:0;

 det = alg_symmetric_definite_det(n, A);
 exp = pow(2, n);
 
 ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(det, exp ,eps), "FAIL: alg_det_symmetric_definite() == %f != %f", det, exp);
 
 return EXIT_SUCCESS;
}
