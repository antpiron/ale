#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.000001, res, delta;

  res = stats_hyper_F(5, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(5, 5, 5, 10) == NaN\n");
  delta = fabs(1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(5, 5, 5, 10) == %f != 1\n", res);

  res = stats_hyper_F(6, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(6, 5, 5, 10) == NaN\n");
  delta = fabs(1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(6, 5, 5, 10) == %f != 1\n", res);

  res = stats_hyper_F(-1, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(-1, 5, 5, 10) == NaN\n");
  delta = fabs(0.0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(-1, 5, 5, 10) == %f != 0.0\n", res);

  res = stats_hyper_F(0, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(0, 5, 5, 10) == NaN\n");
  delta = fabs(0.003968254 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(0, 5, 5, 10) == %f != 0.003968254\n", res);

  res = stats_hyper_F(1, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(1, 5, 5, 10) == NaN\n");
  delta = fabs(0.1031746 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(1, 5, 5, 10) == %f != 0.1031746\n", res);

  res = stats_hyper_F(2, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(2, 5, 5, 10) == NaN\n");
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(2, 5, 5, 10) == %f != 0.5\n", res);

  return EXIT_SUCCESS;
}
