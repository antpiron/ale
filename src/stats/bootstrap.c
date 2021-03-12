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


void
stats_bootstrap_correlated(size_t n, double vec[n],
			   int (*correlation)(size_t i, size_t j, double *r, double *mse, double *pvalue, void *cls),
			   void *cls, double res[n])
{
  size_t max_uncorrelated = 100;
  size_t n_uncorrelated = 0;
  size_t n_correlated = 0;
  size_t *index_uncorrelated =  malloc( sizeof(size_t) * max_uncorrelated );
  size_t *index_correlated =  malloc( sizeof(size_t) * n );
  size_t *index = malloc( sizeof(size_t) * n );
  double r, mse, pvalue;
 
  shuffle_n_size_t(n, index);

  /* Build a set of max_uncorrelated variable */
  index_uncorrelated[ n_uncorrelated++ ] = index[0];
  for (size_t i = n_uncorrelated ; i < n && n_uncorrelated <  max_uncorrelated ; i++)
    {
      int uncorrelated = 1;

      for (size_t u = 0 ; u < n_uncorrelated ; u++)
	{
	  correlation(index[i], index_uncorrelated[u], &r, &mse, &pvalue, cls);
	  if ( pvalue <= 0.05 )
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
      double best_r = 0;
      ssize_t best = -1;
      ssize_t best_mse = -1;
      for (size_t u = 0 ; u < n_uncorrelated ; u++)
	{
	  correlation(index_correlated[i], index_uncorrelated[u], &r, &mse, &pvalue, cls);
	  if ( fabs(best_r) > fabs(r) )
	    {
	      best_r = r;
	      best_mse = mse;
	      best = index_uncorrelated[u];
	    }
	}

      // predict
      // TODO: keep everything in the [0, 1] for p-value or fold FC
      res[index_correlated[i]] = r * vec[best] + stats_norm_rand(0, best_mse);
    }

    
  free(index);
  free(index_uncorrelated);
  free(index_correlated);
}
