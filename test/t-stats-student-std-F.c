#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.001, res, delta;

  res = stats_student_std_F(0, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_student_std_F(0, 10)) == NaN\n");
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_student_std_F(0, 10)) == %f != 0.5\n", res);

  res = stats_student_std_F(1.96, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_student_std_F(1.96, 10)) == NaN\n");
  delta = fabs(0.9607819 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_norm_std_F(1.96, 10)) == %f != 0.9607819\n", res);

  res = stats_student_std_F(-1.96, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_student_std_F(-1.96, 10)) == NaN\n");
  delta = fabs(0.03921812 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_student_std_F(-1.96, 10)) == %f != 0.03921812\n", res);

  return EXIT_SUCCESS;
}
