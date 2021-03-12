#include <stdlib.h>
#include <math.h>
#include "ale/stats.h"


void
shuffle_n_size_t(size_t n, size_t *vec)
{
  for (size_t i = 0 ; i < n ; i++)
    vec[i] = i;

  stats_shuffle(vec, sizeof(size_t), n);
}

enum {
  STATS_CP_PREDICT = 1,
  STATS_CP_R       = 1 << 2,
  STATS_CP_MSE     = 1 << 3,
  STATS_CP_PVALUE  = 1 << 4,

  STATS_CP_ALL     = STATS_CP_PREDICT | STATS_CP_R | STATS_CP_MSE | STATS_CP_PVALUE
};

struct stats_cp_results
{
  double y, r, mse, pvalue;
};
  
void
stats_correlated_permutation(size_t n, double vec[n],
			     int (*predict)(size_t i, size_t j, int flags, double x,
					    struct stats_cp_results *res, void *cls),
			     void *cls, double res[n])
{
  size_t max_uncorrelated = 100;
  size_t n_uncorrelated = 0;
  size_t n_correlated = 0;
  size_t *index_uncorrelated =  malloc( sizeof(size_t) * max_uncorrelated );
  size_t *index_correlated =  malloc( sizeof(size_t) * n );
  size_t *index = malloc( sizeof(size_t) * n );
  struct stats_cp_results predict_res;
  
  shuffle_n_size_t(n, index);

  /* Build a set of max_uncorrelated variable */
  index_uncorrelated[ n_uncorrelated++ ] = index[0];
  for (size_t i = n_uncorrelated ; i < n && n_uncorrelated <  max_uncorrelated ; i++)
    {
      int uncorrelated = 1;

      for (size_t u = 0 ; u < n_uncorrelated ; u++)
	{
	  predict(index[i], index_uncorrelated[u], 0, STATS_CP_PVALUE, &predict_res, cls);
	  if ( predict_res.pvalue <= 0.05 )
	    {
	      uncorrelated = 0;
	      break;
	    }
	}
      if (uncorrelated)
	{
	  index_uncorrelated[ n_uncorrelated++ ] = index[i];
	}
      else
	{
	  index_correlated[ n_correlated++ ] = index[i];
	}
    }
  
  for (size_t i = n_correlated ; i < n ; i++)
    index_correlated[ n_correlated++ ] = index[i];
  
  /* permute uncorrelated */
  shuffle_n_size_t(n_uncorrelated, index);
  for (size_t i = 0 ; i < n_uncorrelated ; i++)
    {
      res[index_uncorrelated[i]] = vec[index[i]];
    }

  // predict uncorrelated
  for (size_t i = 0 ; i < n_correlated ; i++)
    {
      struct stats_cp_results best = { .mse = DBL_MAX };

      for (size_t u = 0 ; u < n_uncorrelated ; u++)
	{
	  predict(index_correlated[i], index_uncorrelated[u], vec[index_uncorrelated[u]],
		  STATS_CP_PREDICT|STATS_CP_MSE, &predict_res, cls);
	  
	  if ( fabs(predict_res.mse) < fabs(best.mse) )
	    {
	      best = predict_res;
	    }
	}

      // predict
      /*
	TODO: Need to be standardized (x_i - mu) / sigma. 
      */
      res[index_correlated[i]] = best.y;
    }

    
  free(index);
  free(index_uncorrelated);
  free(index_correlated);
}
