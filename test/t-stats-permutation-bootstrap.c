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

int predict_ld(size_t i, size_t j, int flags, double x,
		  struct stats_predict_results *res, void *cls)
{
  struct stats_permutation *permutation = cls;
  long diff = labs((long) i - (long) j);
  
  if ( 0 == i % 2 && diff == 2)
    {
      res->pvalue = 0;
      res->mse = 0;
      res->r = 0;
      res->y = -1;
    }
  else
    {
      res->pvalue = 1;
      res->mse = DBL_MAX;
      res->r = 0;
      res->y = 0;
    }
   
  
  return 0;
}


int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN], res[LEN];
  ssize_t corr[LEN];
  int ret;
  struct stats_permutation permutation;

  for (size_t i = 0 ; i < LEN ; i++) 
    x[i] = i;

  // init/destroy
  ret = stats_permutation_correlated_init(&permutation, LEN, x, -1, predict_no_ld, NULL);
  ERROR_FATAL(0 != ret, "FAIL: stats_permutation_correlated_init() != 0\n");
  ERROR_UNDEF_FATAL(STATS_PERMUTATION_CORRELATED != permutation.tag,
		    "FAIL: stats_permutation_correlated_init() tag != STATS_PERMUTATION_CORRELATED\n");
  stats_permutation_destroy(&permutation);


  // set/get
  ret = stats_permutation_correlated_init(&permutation, LEN, x, -1, predict_no_ld, NULL);
  ERROR_FATAL(0 != ret, "FAIL: stats_permutation_correlated_init() != 0\n");

  for (size_t i = 0 ; i < LEN ; i++) 
    corr[i] = (0 == (i % 2))? -1 : i-1;

  stats_permutation_correlated_set(&permutation, corr);
  
  for (size_t i = 0 ; i < LEN ; i++) 
    corr[i] = 0;

  stats_permutation_correlated_get(&permutation, corr);
  
  for (size_t i = 0 ; i < LEN ; i++) 
    {
      ERROR_UNDEF_FATAL_FMT(0 == (i % 2) && -1 != corr[i],
			    "FAIL:  stats_permutation_correlated_get() corr[%zu] == %zu != -1\n",
			    i, corr[i]);
      ERROR_UNDEF_FATAL_FMT(0 != (i % 2) && (i-1) != corr[i],
			    "FAIL:  stats_permutation_correlated_get() corr[%zu] == %zu != %zu\n",
			    i, corr[i], i-1);
    }

  stats_permutation_destroy(&permutation);

  // permutation without LD from set
  ret = stats_permutation_correlated_init(&permutation, LEN, x, -1, predict_no_ld, NULL);
  ERROR_FATAL(0 != ret, "FAIL: stats_permutation_correlated_init() != 0\n");
  
  for (size_t i = 0 ; i < LEN ; i++) 
    corr[i] = -1;

  stats_permutation_correlated_set(&permutation, corr);

  stats_permutation(&permutation, res);
  qsort_r(res, LEN, sizeof(double), 
	  sort_compar_double, NULL);
  for (size_t i = 0 ; i < LEN ; i++) 
    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res[i], i, EPS), "FAIL: from set, res.sorted[%zu] = %f != %zu\n", i, res[i], i); 

  stats_permutation_destroy(&permutation);
 
  // LD without LD ;-)
  ret = stats_permutation_correlated_init(&permutation, LEN, x, -1, predict_no_ld, NULL);
  ERROR_FATAL(0 != ret, "FAIL: stats_permutation_correlated_init() != 0\n");

  stats_permutation_correlated_prepare(&permutation);
  ERROR_UNDEF_FATAL_FMT(LEN != permutation.correlated.n_uncorrelated,
			"FAIL:  n_uncorrelated = %zu != %zu\n",
			permutation.correlated.n_uncorrelated, LEN);
  ERROR_UNDEF_FATAL_FMT(0 != permutation.correlated.n_correlated,
			"FAIL:  n_correlated = %zu != %zu\n",
			permutation.correlated.n_correlated, 0);

  stats_permutation(&permutation, res);

  qsort_r(res, LEN, sizeof(double), 
	  sort_compar_double, NULL);
  for (size_t i = 0 ; i < LEN ; i++) 
    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res[i], i, EPS), "FAIL: res.sorted[%zu] = %f != %zu\n", i, res[i], i); 
   
  stats_permutation_destroy(&permutation);

  
  // LD
  ret = stats_permutation_correlated_init(&permutation, LEN, x, -1, predict_ld, &permutation);
  ERROR_FATAL(0 != ret, "FAIL: stats_permutation_correlated_init() != 0\n");

  stats_permutation_correlated_prepare(&permutation);
  ERROR_UNDEF_FATAL_FMT(LEN / 2 >= permutation.correlated.n_uncorrelated,
			"FAIL:  n_uncorrelated = %zu <= %zu\n",
			permutation.correlated.n_uncorrelated, LEN / 2);
  ERROR_UNDEF_FATAL_FMT(LEN == permutation.correlated.n_uncorrelated,
			"FAIL:  n_uncorrelated = %zu == %zu\n",
			permutation.correlated.n_uncorrelated, LEN);
  ERROR_UNDEF_FATAL_FMT(LEN != permutation.correlated.n_uncorrelated + permutation.correlated.n_correlated,
			"FAIL:  n_correlated + n_uncorrelated = %zu != %zu\n",
			permutation.correlated.n_uncorrelated + permutation.correlated.n_correlated, LEN);
  

  printf("corr = %zu, uncorr = %zu\n", permutation.correlated.n_correlated, permutation.correlated.n_uncorrelated);
  stats_permutation(&permutation, res);
  for (size_t i = 0 ; i < LEN ; i++) 
    {
      ERROR_UNDEF_FATAL_FMT(1 == (i % 2) && res[i] < 0, 
			    "FAIL:  res[%zu] = %d <= 0\n",
			    i, res[i]);
    }

  for (size_t i = 0 ; i < LEN ; i++) 
    printf(" %f", res[i]);
  printf("\n");

  printf("get\n");
  stats_permutation_correlated_get(&permutation, corr);
  for (size_t i = 0 ; i < LEN ; i++) 
    printf(" %zd", corr[i]);
  printf("\n");


  stats_permutation_destroy(&permutation);
   
  return EXIT_SUCCESS;
}
