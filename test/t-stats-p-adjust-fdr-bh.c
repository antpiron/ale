#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double p[LEN], padj[LEN];
  double res;
  double eps = 0.000000001;

  p[0] = 0.001;
  for (int i = 1 ; i < LEN ; i++)
    p[i] = p[i-1] + 0.001;

  res = stats_p_adjust_fdr_bh(LEN, p, padj);
  ERROR_UNDEF_FATAL_FMT(res < 0, "FAIL: stats_p_adjust_fdr_bh()  res = %d != 0\n", res);
  for (int i = 0 ; i < LEN ; i++)
    {
      printf("%f ", padj[i]);
      ERROR_UNDEF_FATAL_FMT(p[i] > padj[i] , "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f < %f\n", i, padj[i], p[i]);
      ERROR_UNDEF_FATAL_FMT(padj[i] > 1 + eps, "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f > 1\n", i, padj[i]);
      ERROR_UNDEF_FATAL_FMT(fabs(padj[i] - 0.1) > eps, "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f != 0.1\n", i, padj[i]);
    }

  
  return EXIT_SUCCESS;
}
