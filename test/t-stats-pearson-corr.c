#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (1000)
  double x[LEN] = {0.0};
  double y[LEN] = {0.0};
  double res, exp, delta, pvalue;
  double eps = 0.0001;
  int ret;

  ret = stats_pearson_corr(LEN, x, y, &res, &pvalue);
  ERROR_UNDEF_FATAL_FMT(-1 != ret, "FAIL: stats_stats_pearson_corr() == %d != -1\n", ret);

  for (int i = 0 ; i < LEN ; i++)
    {
      x[i] = i;
      y[i] = 2*i;
    }

  ret = stats_pearson_corr(LEN, x, y, &res, &pvalue);
  ERROR_UNDEF_FATAL_FMT(-1 == ret,
			"FAIL: stats_stats_pearson_corr() == %d != 0\n", ret);
  exp = 1.0;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: stats_pearson_corr() == %f != %f. delta = %f\n",
			res, exp, delta);
  exp = 0.0;
  delta = fabs(exp - pvalue);
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: stats_pearson_corr().pval == %f != 0\n", pvalue);
  
  for (long i = 0 ; i < LEN ; i++)
    {
      x[i] = stats_unif_rand(0,100);
      y[i] = stats_unif_rand(0,100);
    }

  ret = stats_pearson_corr(LEN, x, y, &res, &pvalue);
  ERROR_UNDEF_FATAL_FMT(-1 == ret, "FAIL: stats_stats_pearson_corr() == %d != 0\n", ret);
  exp = 0.0;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(pvalue < 0.01 && delta >= eps,
			"FAIL: stats_pearson_corr() == %f != %f. pval = %f\n",
			res, exp, pvalue);
  ERROR_UNDEF_FATAL_FMT(pvalue < 0.01,
			"FAIL: stats_pearson_corr().pval == %f < 0.05\n", pvalue);
  
  
  return EXIT_SUCCESS;
}
