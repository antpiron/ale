#include <stdlib.h>
#include "ale/stats.h"



void
stats_bootstrap_correlated(size_t n, double vec[n],
			   int (*correlation)(size_t i, size_t j, double *r, double *mse, double *pvalue, void *cls),
			   void *cls, double res[n])
{
  size_t max_uncorrelated = 100;
  size_t n_uncorrelated = 0;
  size_t *index_uncorrelated =  malloc( sizeof(size_t) * max_uncorrelated );
  size_t *index = malloc( sizeof(size_t) * n );
  double r, mse, pvalue;
 
  for (size_t i = 0 ; i < n ; i++)
    index[i] = i;

  // TODO: implement a shuffle which keep only the top
  stats_shuffle(index, sizeof(size_t), n);

  /* Build a set of max_uncorrelated variable */
  index_uncorrelated[ n_uncorrelated++ ] = index[0];
  for (size_t i = n_uncorrelated ; i < n && n_uncorrelated <  max_uncorrelated ; i++)
    {
      int uncorrelated = 1;

      for (size_t u = 0 ; u < n_uncorrelated ; u++)
	{
	  correlation(index[i], index[u], &r, &mse, &pvalue, cls);
	  if ( pvalue <= 0.05 )
	    {
	      uncorrelated = 0;
	      break;
	    }
	}
      if (uncorrelated)
	index_uncorrelated[ n_uncorrelated++ ] = index[i];
    }

  for (size_t i = 0, iu = 0 ; i < n ; i++)
    {
      if ( iu < n_uncorrelated && index[i] == index_uncorrelated[iu] )
	{
	  res[i] = vec[ index[i] ];
	  iu++;
	  continue;
	}

      for (size_t u = 0 ; u < n_uncorrelated ; u++)
	{
	  correlation(index[i], index[u], &r, &mse, &pvalue, cls);
	  
	}
      
    }

    
  free(index);
  free(index_uncorrelated);
}
