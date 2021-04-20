#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (1000000)
  double *x = malloc(sizeof(double)*LEN);
  double eps = 0.01, res;
  double count = 0;
  double alpha = 10, beta = 4;
  double ralpha, rbeta;
  int ret;

  for (int iter = 0 ; iter < 4 ; iter++)
    {
      alpha = stats_unif_std_rand() * 100;
      beta = stats_unif_std_rand() * 100;
      
      for (int i = 0 ; i < LEN ; i++)
	res = x[i] = stats_beta_rand(alpha, beta);
      
      ret = stats_beta_fit_mle(LEN, x, &ralpha, &rbeta);
      ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(ralpha, alpha, eps), "FAIL: stats_beta_fit_mle(), alpha == %f != %f\n",
			    ralpha, alpha);
      ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(rbeta, beta, eps), "FAIL: stats_beta_fit_mle(), beta == %f != %f\n",
			    rbeta, beta);
    }
  free(x);

  return EXIT_SUCCESS;
}