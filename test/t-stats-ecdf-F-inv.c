#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  struct stats_ecdf ecdf;
  size_t n = 10;
  double x[n];
  double res, expected;
  double eps = 0.0000001;

  for (size_t i = 0 ; i < n ; i++)
    x[i] = i+1;

  stats_ecdf_init(&ecdf, n, x);
  ERROR_UNDEF_FATAL_FMT(ecdf.n != n, "FAIL: stats_ecdf_init() %zu != %zu\n", ecdf.n, n);

  res = stats_ecdf_F(&ecdf, n+1);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, 1.0d, eps) , "FAIL: stats_ecdf_F(%zu) = %f != 1.0\n", n+1, res);

  res = stats_ecdf_F_inv(&ecdf, 1);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, n, eps) , "FAIL: stats_ecdf_F_inv(1) = %f != %zu\n", res, n);

  res = stats_ecdf_F_inv(&ecdf, 0);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, 0, eps) , "FAIL: stats_ecdf_F_inv(0) = %f != %zu\n", res, 0);

  stats_ecdf_destroy(&ecdf);
  
  
  return EXIT_SUCCESS;
}
