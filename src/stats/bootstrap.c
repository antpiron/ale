#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ale/stats.h"
#include "ale/error.h"


enum {
      STATS_PERMUTATION,
      STATS_PERMUTATION_CORRELATED
};

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

struct stats_permutation
{
  int tag;
  size_t n;
  double *vec;
  union {
    struct {
      size_t max_uncorrelated;
      size_t n_uncorrelated;
      size_t n_correlated;
      size_t *index_uncorrelated;
      size_t *index_correlated;
      ssize_t *index_best_predictor;
      int (*predict)(size_t i, size_t j, int flags, double x,
		     struct stats_cp_results *res, void *cls);
      void *cls;
    } correlated;
  };
};

void
shuffle_n_size_t(size_t n, size_t *vec)
{
  for (size_t i = 0 ; i < n ; i++)
    vec[i] = i;

  stats_shuffle(vec, sizeof(size_t), n);
}

void
stats_permutation_init(struct stats_permutation *p, size_t n, double vec[n])
{
  p->tag = STATS_PERMUTATION;
  p->n = n;
  p->vec = vec;
}

int
stats_permutation_correlated_init(struct stats_permutation *p, size_t n, double vec[n],
				  int (*predict)(size_t i, size_t j, int flags, double x,
						 struct stats_cp_results *res, void *cls),
				  void *cls)
{
  stats_permutation_init(p, n, vec);
  
  p->tag = STATS_PERMUTATION_CORRELATED;
  p->correlated.n_uncorrelated = p->correlated.n_correlated = 0;
  p->correlated.predict = predict;
  p->correlated.cls = cls;
  p->correlated.max_uncorrelated = 100;
  p->correlated.index_uncorrelated =  malloc( sizeof(size_t) * p->correlated.max_uncorrelated );
  p->correlated.index_correlated =  malloc( sizeof(size_t) * n );
  p->correlated.index_best_predictor =  malloc( sizeof(size_t) * n );
  
  return 0;
}

void
stats_permutation_correlated_destroy(struct stats_permutation *p)
{
  free(p->correlated.index_uncorrelated);
  free(p->correlated.index_correlated);
  free(p->correlated.index_best_predictor);
}

void
stats_permutation_destroy(struct stats_permutation *p)
{
  if ( STATS_PERMUTATION_CORRELATED == p->tag )
    stats_permutation_correlated_destroy(p);
}

int
stats_permutation_correlated_prepare(struct stats_permutation *p)
{
  ERROR_UNDEF_RET(STATS_PERMUTATION == p->tag, -1);
  
  size_t *index = malloc( sizeof(size_t) * p->n );
  struct stats_cp_results predict_res;
  
  shuffle_n_size_t(p->n, index);
   
  /* Build a set of max_uncorrelated variable */
  p->correlated.index_uncorrelated[ p->correlated.n_uncorrelated++ ] = index[0];
  for (size_t i = p->correlated.n_uncorrelated ; i < p->n && p->correlated.n_uncorrelated <  p->correlated.max_uncorrelated ; i++)
    {
      int uncorrelated = 1;
       
      for (size_t u = 0 ; u < p->correlated.n_uncorrelated ; u++)
	{
	  p->correlated.predict(index[i], p->correlated.index_uncorrelated[u], 0, STATS_CP_PVALUE, &predict_res, p->correlated.cls);
	  if ( predict_res.pvalue <= 0.05 )
	    {
	      uncorrelated = 0;
	      break;
	    }
	}
      if (uncorrelated)
	{
	  p->correlated.index_uncorrelated[ p->correlated.n_uncorrelated++ ] = index[i];
	}
      else
	{
	  p->correlated.index_correlated[ p->correlated.n_correlated++ ] = index[i];
	}
    }

  for (size_t i = p->correlated.n_correlated ; i < p->n ; i++)
    p->correlated.index_correlated[ p->correlated.n_correlated++ ] = index[i];
  
  // predict uncorrelated
  for (size_t i = 0 ; i <  p->correlated.n_correlated ; i++)
    {
      struct stats_cp_results best = { .mse = DBL_MAX };
      ssize_t best_u = -1;

      for (size_t u = 0 ; u <  p->correlated.n_uncorrelated ; u++)
	{
	  p->correlated.predict(p->correlated.index_correlated[i],  p->correlated.index_uncorrelated[u],  p->vec[ p->correlated.index_uncorrelated[u]],
				STATS_CP_MSE, &predict_res,  p->correlated.cls);
	  
	  if ( fabs(predict_res.mse) < fabs(best.mse) )
	    {
	      best = predict_res;
	      best_u = u;
	    }
	}

      p->correlated.index_best_predictor[ p->correlated.index_correlated[i] ] = p->correlated.index_uncorrelated[best_u];
    }

 
  free(index);
  
  return 0;
}




int
stats_permutation(struct stats_permutation *p, size_t n, double res[n])
{
  if ( STATS_PERMUTATION == p->tag )
    {
      memcpy(res, p->vec, sizeof(double) * n);
      stats_shuffle(res, sizeof(double), n);
    }
  else if ( STATS_PERMUTATION_CORRELATED == p->tag )
    {
      size_t *index = malloc( sizeof(size_t) * p->correlated.n_uncorrelated );

      shuffle_n_size_t(p->correlated.n_uncorrelated, index);

      for (size_t i = 0 ; i < p->correlated.n_uncorrelated ; i++)
	res[ p->correlated.index_uncorrelated[i] ] = p->vec[ p->correlated.index_uncorrelated[index[i]] ];

      for (size_t i = 0 ; i < p->correlated.n_correlated ; i++)
	{
	  size_t to_predict = p->correlated.index_correlated[i];
	  size_t predictor = p->correlated.index_best_predictor[to_predict];
	  struct stats_cp_results predict_res;

	  p->correlated.predict(to_predict,  predictor, p->vec[ predictor ],
				STATS_CP_PREDICT, &predict_res,  p->correlated.cls);
	  p->vec[to_predict] = predict_res.y;
	}

      free(index);
    }

  return 0;
}


