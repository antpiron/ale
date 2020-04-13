#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/stats-normalize.h"

int
main(int argc, char *argv[argc])
{
#define N (10)
#define M (3)
  double (*mat)[M][N] = malloc(N*M * sizeof(double));
  double beta[N];
  size_t ref[M] = {0, 1, 2};
  double eps = 0.05;
  double res, delta;

  for (int i = 0 ; i < M ; i++)
    (*mat)[i][0] = 2*(i + 100);

  for (int i = 0 ; i < M ; i++)
    for (int j = 1 ; j < N ; j++)
    {
      (*mat)[i][j] =  (*mat)[i][0] + stats_norm_std_rand();
    }

  stats_normalize_samples_ls(M, N, M, *mat, ref, beta);
  for (int i = 0 ; i < M ; i++)
    for (int j = 0 ; j < N ; j++)
      {
	double exp =  stats_mean(N, &(*mat)[i]);
	double delta = fabs((*mat)[i][j] - exp) / exp;
	ERROR_UNDEF_FATAL_FMT(delta > eps, "FAIL: stats_normalize_samples_ls() %f != %f (delta = %f)\n",
			      (*mat)[i][j], exp, delta); 
      }

  free(mat);

  return EXIT_SUCCESS;
}
