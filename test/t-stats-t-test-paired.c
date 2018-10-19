#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (10000)
  double x[LEN] = {0.0};
  double y[LEN] = {0.0};
  double res, exp, delta;
  struct stats_t_test data;
  double eps = 0.01;

  for (int i = 0 ; i < LEN ; i++)
    {
      x[i] = stats_norm_std_rand();
      y[i] = stats_norm_std_rand();
    }

  res = stats_t_test_paired(LEN, x, y, 0, 0, &data); 
  double m = stats_mean(LEN, x);
  ERROR_UNDEF_FATAL_FMT(res < eps, "FAIL: stats_t_test() == %f < %f. Mean = %f \n",
			res, eps, m);
  for (int i = 0 ; i < LEN ; i++)
    {
      x[i] = stats_norm_rand(1, 1);
      y[i] = stats_norm_rand(2, 1);
    }

  res = stats_t_test_paired(LEN, x, y, 0, 0, &data); 
  m = stats_mean(LEN, x);
  ERROR_UNDEF_FATAL_FMT(res > eps, "FAIL: stats_t_test() == %f < %f. Mean = %f \n",
			res, eps, m);
  
  
  return EXIT_SUCCESS;
}
