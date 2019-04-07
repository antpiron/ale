#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.005, res, delta, alpha = 1, beta = 1;

  res = stats_gamma_F(2, alpha, beta);
  ERROR_UNDEF_FATAL_FMT(isnan(res), "FAIL: count(stats_gamma_F(2, %f, %f) == NaN\n", alpha, beta);
  delta = fabs(0.8646647 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_gamma_F(2, %f, %f)) == %f != 0.5\n",
			alpha, beta, res);

  alpha = 2;
  res = stats_gamma_F(2, alpha, beta);
  ERROR_UNDEF_FATAL_FMT(isnan(res), "FAIL: count(stats_gamma_F(2, %f, %f) == NaN\n", alpha, beta);
  delta = fabs(0.5939942 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_gamma_F(2, %f, %f)) == %f != 0.5\n",
			alpha, beta, res);

  alpha = 2; beta = 2;
  res = stats_gamma_F(2, alpha, beta);
  ERROR_UNDEF_FATAL_FMT(isnan(res), "FAIL: count(stats_gamma_F(2, %f, %f) == NaN\n", alpha, beta);
  delta = fabs(0.9084218 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_gamma_F(2, %f, %f)) == %f != 0.5\n",
			alpha, beta, res);

 
  return EXIT_SUCCESS;
}
