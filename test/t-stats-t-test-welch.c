#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN1 (10000)
#define LEN2 (9000)
  double x[LEN1] = {0.0};
  double y[LEN2] = {0.0};
  double res, exp, delta;
  struct stats_t_test data;
  double eps = 0.01;

  for (int i = 0 ; i < LEN1 ; i++)
    x[i] = stats_norm_std_rand();
  for (int i = 0 ; i < LEN2 ; i++)
    y[i] = stats_norm_std_rand();
      

  res = stats_t_test_welch(LEN1, x, LEN2, y, 0, 0, &data); 
  double mx = stats_mean(LEN1, x);
  double my = stats_mean(LEN2, y);
  ERROR_UNDEF_FATAL_FMT(res < eps,
			"FAIL: stats_t_test_welch() == %f < %f. Mean x = %f, y = %f \n",
			res, eps, mx, my);
  for (int i = 0 ; i < LEN1 ; i++)
    x[i] = stats_norm_rand(1, 1);
  for (int i = 0 ; i < LEN2 ; i++)
    y[i] = stats_norm_rand(2, 1);

  res = stats_t_test_welch(LEN1, x, LEN2, y, 0, 0, &data); 
  mx = stats_mean(LEN1, x);
  my = stats_mean(LEN2, y);
  ERROR_UNDEF_FATAL_FMT(res > eps,
			"FAIL: stats_t_test_welch() == %f < %f. Mean = %f, y = %f \n",
			res, eps, mx, my);
  
  
  return EXIT_SUCCESS;
}
