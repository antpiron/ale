
#include "ale/stats-normalize.h"
#include "ale/stats.h"
#include "ale/error.h"

#include <stdlib.h>
#include <math.h>

int
stats_normalize_samples_ls(size_t m, size_t n, size_t r,
			   const double mat[m][n], const size_t ref[r], double beta[n],
			   unsigned int options)
{
  double *w = malloc( r * sizeof(double) );
  int ret = -1;

  if ( STATS_WEIGHT_VARIANCE & options )
    {
      for (size_t i = 0 ; i < r ; i++)
	{
	  double var = stats_var(n, mat[ref[i]]);
	  ERROR_CUSTOM_GOTO(0 == var, NORMALIZE_EDIVZERO, ERROR_LS);
	  w[i] = 1.0d / var;
	}
    }
  else
    {
      for (size_t i = 0 ; i < r ; i++)
	{
	  double mean = stats_mean(n, mat[ref[i]]);
	  ERROR_CUSTOM_GOTO(0 == mean, NORMALIZE_EDIVZERO, ERROR_LS);
	  w[i] = 1.0d / mean;
	}
    }

  // TODO: switch order of loop for cache
  for (size_t s = 0 ; s < n ; s++)
    {
      double denom = 0;
      double num = 0;
      for (size_t i = 0 ; i < r ; i++)
	{
	  double x =  mat[ref[i]][s];
	  num += x;
	  denom += w[i] * x* x ;
	}
      
      ERROR_CUSTOM_GOTO(0 == denom, NORMALIZE_EDIVZERO, ERROR_LS);

      // ERROR_UNDEF_RET(0 == denom, -1);
      beta[s] = num / denom;
    }
  ret = 0;
  
 ERROR_LS:
  free(w);

  return ret;
}

int stats_normalize_samples_poisson(size_t m, size_t n, size_t r,
				    const double mat[m][n], const size_t ref[r], double beta[n])
{
  double *mumu = malloc( r * sizeof(double) );
  int ret = -1;
    
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
	  double x = mat[ref[i]][s];
	  ERROR_CUSTOM_GOTO(0 == x, NORMALIZE_EDIVZERO, ERROR_POISSON);
	  
	  num += mumu[i] / x;
	  denom += x;
	}
      beta[s] = sqrt(num / denom);
    }
  ret = 0;

 ERROR_POISSON:
  free(mumu);
  
  return ret;
}

void
stats_normalize_samples(size_t m, size_t n, double dst[m][n],
			const double src[m][n], const double beta[n])
{
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ;  j < n ; j++)
      dst[i][j] = beta[j] * src[i][j];     
}
