
#include "ale/stats-normalize.h"
#include "ale/stats.h"
#include "ale/error.h"

#include <stdlib.h>
#include <math.h>

int
stats_normalize_samples_ls(size_t m, size_t n, size_t r,
			   const double mat[m][n], size_t ref[r], double beta[n])
{
  double *w = malloc( r * sizeof(double) );
    
  for (size_t i = 0 ; i < r ; i++)
    w[i] = 1.0d / stats_mean(n, mat[ref[i]]);

  // TODO: switch order of loop for cache
  for (size_t s = 0 ; s < n ; s++)
    {
      double denom = 0;
      double num = 0;
      for (size_t i = 0 ; i < r ; i++)
	{
	  num += mat[i][s];
	  denom += w[i] * mat[i][s] * mat[i][s] ;
	}
      beta[s] = num / denom;
    }

  free(w);
  
  return 0;
}

int stats_normalize_samples_poisson(size_t m, size_t n, size_t r,
				    const double mat[m][n], size_t ref[r], double beta[n])
{
  double *mumu = malloc( r * sizeof(double) );
    
  for (size_t i = 0 ; i < r ; i++)
    {
      double mu = stats_mean(n, mat[ref[i]]);
      mumu[i] = mu * mu;
    }

  // TODO: switch order of loop for cache
  for (size_t s = 0 ; s < n ; s++)
    {
      double denom = 0;
      double num = 0;
      for (size_t i = 0 ; i < r ; i++)
	{
	  double divisor = mat[i][s];
	  ERROR_UNDEF_RET(0 == divisor, -1);
	  num += mumu[i] / divisor;
	  denom += mat[i][s];
	}
      beta[s] = sqrt(num / denom);
    }

  
  return 0;
}
