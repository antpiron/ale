#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN] = {0.0};
  double res, exp, delta;
  double eps = 0.0001;

  for (int i = 0 ; i < LEN ; i++)
    x[i] = stats_unif_std_rand();

  res = stats_entropy(LEN, x);
  exp = 0.0;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_entropy() == %f != %f. delta = %f\n", res, exp, delta);
  
  
  return EXIT_SUCCESS;
}
