#define _GNU_SOURCE
#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/sort.h"
#include "ale/math.h"
#include <math.h>

#define ORDER 16
#define EPS (0.0000000001d)

int predict_no_ld(size_t i, size_t j, int flags, double x,
		  struct stats_predict_results *res, void *cls)
{
  res->pvalue = 1;
  res->mse = DBL_MAX;
  res->r = 0;
  res->y = 0; 
  
  return 0;
}


int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN], res[LEN];
  size_t count;
  size_t count_min = (size_t) sqrt(LEN);
  int ret;
  struct stats_permutation permutation;

  for (size_t i = 0 ; i < LEN ; i++) 
    x[i] = i;

  stats_permutation_init(&permutation, LEN, x);

  stats_permutation(&permutation, LEN, res);

  qsort_r(res, LEN, sizeof(double), 
	  sort_compar_double, NULL);
  for (size_t i = 0 ; i < LEN ; i++) 
    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res[i], i, EPS), "FAIL: res.sorted[%zu] = %f != %zu", i, res[i], i); 

  stats_permutation_destroy(&permutation);


  // LD without LD ;-)
  ret = stats_permutation_correlated_init(&permutation, LEN, x, -1, predict_no_ld, NULL);
  ERROR_FATAL(0 != ret, "FAIL: stats_permutation_correlated_init() != 0\n");
  ERROR_UNDEF_FATAL(STATS_PERMUTATION_CORRELATED != permutation.tag,
		    "FAIL: stats_permutation_correlated_init() tag != STATS_PERMUTATION_CORRELATED\n");

  stats_permutation_correlated_prepare(&permutation);
  ERROR_UNDEF_FATAL_FMT(LEN != permutation.correlated.n_uncorrelated,
			"FAIL:  n_uncorrelated = %zu != %zu\n",
			permutation.correlated.n_uncorrelated, LEN);
  ERROR_UNDEF_FATAL_FMT(0 != permutation.correlated.n_correlated,
			"FAIL:  n_correlated = %zu != %zu\n",
			permutation.correlated.n_correlated, 0);

  stats_permutation(&permutation, LEN, res);

  qsort_r(res, LEN, sizeof(double), 
	  sort_compar_double, NULL);
  for (size_t i = 0 ; i < LEN ; i++) 
    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res[i], i, EPS), "FAIL: res.sorted[%zu] = %f != %zu\n", i, res[i], i); 

  stats_permutation_destroy(&permutation);


  return EXIT_SUCCESS;
}
