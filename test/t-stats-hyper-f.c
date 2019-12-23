#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.000001, res, delta;


  
  res = stats_hyper_f(1, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_f(1, 5, 5, 10) == NaN\n");
  delta = fabs(0.09920635 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_f(1, 5, 5, 10) == %f != 0.09920635\n", res);

  res = stats_hyper_f(2, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_f(2, 5, 5, 10) == NaN\n");
  delta = fabs(0.3968254 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_f(2, 5, 5, 10) == %f != 0.3968254\n", res);

  res = 0;
  for (long k = 0 ; k < 11 ; k++)
    {
      res += stats_hyper_f(k, 5, 5, 10);
    }
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: sum(stats_hyper_f(k, 5, 5, 10)) == NaN\n");
  delta = fabs(1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: sum(stats_hyper_f(k, 5, 5, 10)) == %f != 1\n", res);

  return EXIT_SUCCESS;
}
