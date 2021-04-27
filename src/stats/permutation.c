#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ale/stats.h"
#include "ale/error.h"
#include "ale/matrix.h"




static void
shuffle_n_size_t(size_t n, size_t *vec)
{
  for (size_t i = 0 ; i < n ; i++)
    vec[i] = i;

  stats_shuffle(vec, n, sizeof(size_t));
}

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  void									\
  stats_permutation_init##SUFFIX(struct stats_permutation##SUFFIX *p, size_t n, TYPE vec[n]) \
  {									\
    p->tag = STATS_PERMUTATION;						\
    p->n = n;								\
    p->vec = vec;							\
  }									\
									\
  int									\
  stats_permutation_correlated_init##SUFFIX(struct stats_permutation##SUFFIX *p, \
					    size_t n, TYPE vec[n], ssize_t max_uncorrelated, \
					    int (*predict)(size_t i, size_t j, int flags, TYPE x, \
							   struct stats_predict_results##SUFFIX *res, void *cls), \
					    void *cls)			\
  {									\
    stats_permutation_init##SUFFIX(p, n, vec);				\
									\
    p->tag = STATS_PERMUTATION_CORRELATED;				\
    p->correlated.n_uncorrelated = p->correlated.n_correlated = 0;	\
    p->correlated.predict = predict;					\
    p->correlated.cls = cls;						\
    p->correlated.max_uncorrelated = (-1 == max_uncorrelated)? n : (size_t) max_uncorrelated; \
    p->correlated.index_uncorrelated =  malloc( sizeof(size_t) * p->correlated.max_uncorrelated ); \
    p->correlated.index_correlated =  malloc( sizeof(size_t) * n );	\
    p->correlated.index_best_predictor =  malloc( sizeof(size_t) * n );	\
									\
    return 0;								\
  }									\
									\
  int stats_predict_from_values##SUFFIX(size_t i, size_t j,		\
					int flags, TYPE x,		\
					struct stats_predict_results##SUFFIX *res, \
					void *cls)			\
  {									\
    /* TODO: implement matrix as long double */				\
    struct matrix *mat = cls;						\
    double (*m)[mat->n] = (void*) mat->data;				\
    double pvalue, rho, rho_abs;					\
									\
    stats_pearson_corr(mat->n, m[i], m[j], &pvalue, &rho);		\
    res->pvalue = pvalue;						\
    res->r = rho;							\
    rho_abs = fabs(rho);						\
    res->y = rho_abs * x + (1 - rho_abs) * stats_unif_std_rand();	\
									\
    return 0;								\
  }  									\
									\
  static								\
  void									\
  stats_permutation_correlated_destroy##SUFFIX(struct stats_permutation##SUFFIX *p) \
  {									\
    free(p->correlated.index_uncorrelated);				\
    free(p->correlated.index_correlated);				\
    free(p->correlated.index_best_predictor);				\
  }									\
									\
  void									\
  stats_permutation_destroy##SUFFIX(struct stats_permutation##SUFFIX *p) \
  {									\
    if ( STATS_PERMUTATION_CORRELATED == p->tag )			\
      stats_permutation_correlated_destroy##SUFFIX(p);			\
  }									\
									\
  int									\
  stats_permutation_correlated_prepare##SUFFIX(struct stats_permutation##SUFFIX *p) \
  {									\
    ERROR_UNDEF_RET(STATS_PERMUTATION == p->tag, -1);			\
									\
    size_t *index = malloc( sizeof(size_t) * p->n );			\
    struct stats_predict_results##SUFFIX predict_res;			\
									\
    shuffle_n_size_t(p->n, index);					\
									\
    /* Build a set of max_uncorrelated variable */			\
    p->correlated.index_uncorrelated[ p->correlated.n_uncorrelated++ ] = index[0]; \
    for (size_t i = p->correlated.n_uncorrelated ; i < p->n && p->correlated.n_uncorrelated <  p->correlated.max_uncorrelated ; i++) \
      {									\
	int uncorrelated = 1;						\
									\
	for (size_t u = 0 ; u < p->correlated.n_uncorrelated ; u++)	\
	  {								\
	    p->correlated.predict(index[i], p->correlated.index_uncorrelated[u], 0, STATS_PFLAGS_PVALUE, &predict_res, p->correlated.cls); \
	    if ( predict_res.pvalue <= 0.05 )				\
	      {								\
		uncorrelated = 0;					\
		break;							\
	      }								\
	  }								\
	if (uncorrelated)						\
	  {								\
	    p->correlated.index_uncorrelated[ p->correlated.n_uncorrelated++ ] = index[i]; \
	  }								\
	else								\
	  {								\
	    p->correlated.index_correlated[ p->correlated.n_correlated++ ] = index[i]; \
	  }								\
      }									\
									\
    for (size_t i = p->correlated.n_correlated + p->correlated.n_uncorrelated ; i < p->n ; i++)	\
      p->correlated.index_correlated[ p->correlated.n_correlated++ ] = index[i]; \
									\
    /* Correlated indexes */						\
    for (size_t i = 0 ; i <  p->correlated.n_correlated ; i++)		\
      {									\
	struct stats_predict_results##SUFFIX best = { .mse = DBL_MAX };	\
	ssize_t best_u = -1;						\
									\
	for (size_t u = 0 ; u <  p->correlated.n_uncorrelated ; u++)	\
	  {								\
	    p->correlated.predict(p->correlated.index_correlated[i],  p->correlated.index_uncorrelated[u],  p->vec[ p->correlated.index_uncorrelated[u]], \
				  STATS_PFLAGS_MSE, &predict_res,  p->correlated.cls); \
									\
	    if ( fabs##SUFFIX(predict_res.mse) < fabs##SUFFIX(best.mse) ) \
	      {								\
		best = predict_res;					\
		best_u = u;						\
	      }								\
	  }								\
									\
	p->correlated.index_best_predictor[ p->correlated.index_correlated[i] ] = p->correlated.index_uncorrelated[best_u]; \
      }									\
									\
									\
    free(index);							\
									\
    return 0;								\
  }									\
									\
  void									\
  stats_permutation_correlated_get##SUFFIX(struct stats_permutation##SUFFIX *p, ssize_t correlated[p->n]) \
  {									\
    for (size_t i = 0 ; i <  p->correlated.n_correlated ; i++)		\
      {									\
	correlated[p->correlated.index_correlated[i]] = p->correlated.index_best_predictor[ p->correlated.index_correlated[i] ]; \
      }									\
    for (size_t i = 0 ; i <  p->correlated.n_uncorrelated ; i++)	\
      {									\
	correlated[p->correlated.index_uncorrelated[i]] = -1;		\
      }									\
  }									\
									\
  void									\
  stats_permutation_correlated_set##SUFFIX(struct stats_permutation##SUFFIX *p, ssize_t correlated[p->n]) \
  {									\
    p->correlated.n_correlated = p->correlated.n_uncorrelated = 0;	\
									\
    for (size_t i = 0 ; i <  p->n ; i++)				\
      {									\
         ssize_t predictor = correlated[i];		\
	 if ( 0 <= predictor)						\
	   {								\
	     p->correlated.index_best_predictor[i] = predictor;		\
	     p->correlated.index_correlated[ p->correlated.n_correlated++ ] = i; \
	   }								\
	 else								\
	   p->correlated.index_uncorrelated[ p->correlated.n_uncorrelated++ ] = i; \
      }									\
  }									\
									\
  int									\
  stats_permutation##SUFFIX(struct stats_permutation##SUFFIX *p, TYPE res[p->n]) \
  {									\
    size_t n = p->n;							\
  									\
    if ( STATS_PERMUTATION == p->tag )					\
      {									\
	memcpy(res, p->vec, sizeof(TYPE) * n);				\
	stats_shuffle(res, n, sizeof(TYPE));				\
      }									\
    else if ( STATS_PERMUTATION_CORRELATED == p->tag )			\
      {									\
	size_t *index = malloc( sizeof(size_t) * p->correlated.n_uncorrelated ); \
									\
	shuffle_n_size_t(p->correlated.n_uncorrelated, index);		\
									\
	for (size_t i = 0 ; i < p->correlated.n_uncorrelated ; i++)	\
	  res[ p->correlated.index_uncorrelated[i] ] = p->vec[ p->correlated.index_uncorrelated[index[i]] ]; \
									\
	for (size_t i = 0 ; i < p->correlated.n_correlated ; i++)	\
	  {								\
	    size_t to_predict = p->correlated.index_correlated[i];	\
	    size_t predictor = p->correlated.index_best_predictor[to_predict]; \
	    struct stats_predict_results##SUFFIX predict_res;		\
	    								\
	    p->correlated.predict(to_predict,  predictor, p->vec[ predictor ], \
				  STATS_PFLAGS_PREDICT, &predict_res,  p->correlated.cls); \
	    res[to_predict] = predict_res.y;				\
	    printf("predict %zu, %zu = %f\n", to_predict, predictor, (double) predict_res.y); \
	  }								\
									\
	free(index);							\
      }									\
									\
    return 0;								\
  }


GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
