#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.000001, res, delta;


  
  res = stats_binom_f(-1, 1, 0.5);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_binom_f(-1, 1, 0.5) == NaN\n");
  delta = fabs(0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_binom_f(-1, 1, 0.5) == %f != 0\n", res);

  res = stats_binom_f(2, 1, 0.5);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_binom_f(2, 1, 0.5) == NaN\n");
  delta = fabs(0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_binom_f(2, 1, 0.5) == %f != 0\n", res);

  res = stats_binom_f(0, 1, 0.5);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_binom_f(0, 1, 0.5) == NaN\n");
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_binom_f(0, 1, 0.5) == %f != 0.5\n", res);

  res = stats_binom_f(1, 2, 0.5);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_binom_f(1, 2, 0.5) == NaN\n");
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_binom_f(1, 2, 0.5) == %f != 0.5\n", res);

  res = 0;
  for (long k = 0 ; k < 11 ; k++)
    {
      res += stats_binom_f(k, 10, 0.5);
    }
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: sum(stats_binom_f(k, 10, 0.5)) == NaN\n");
  delta = fabs(1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: sum(stats_binom_f(k, 10, 0.5)) == %f != 1\n", res);

  return EXIT_SUCCESS;
}
