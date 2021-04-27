#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  									\
  struct stats_ecdf##SUFFIX						\
  {									\
    size_t n;								\
    TYPE *x;								\
    size_t *index;							\
  };									\
  									\
  void									\
  stats_ecdf_init##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, size_t n,	\
			  TYPE x[n])					\
  {									\
    ecdf->n= n;								\
    ecdf->x = x;							\
    ecdf->index = malloc(n * sizeof(size_t));				\
    sort_q_indirect(ecdf->index, x, n, sizeof(TYPE),			\
		    sort_compar_double#SUFFIX, NULL);			\
  }									\
  									\
  void									\
  stats_ecdf_destroy##SUFFIX(struct stats_ecdf##SUFFIX *ecdf)		\
  {									\
    free(ecdf->index);							\
  }									\
  									\
  TYPE									\
  stats_ecdf_f##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE x)		\
  {									\
    return 0;								\
  }									\
  									\
  TYPE									\
  stats_ecdf_F##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE x)		\
  {									\
    ssize_t l = 0, r = ecdf->n - 1;					\
    									\
    if ( x < ecdf->x[ ecdf->index[l] ])					\
      return 0;								\
    else if ( x >= ecdf->x[ ecdf->index[r] ])				\
      return 1;								\
    else								\
      while (l < r)							\
	{								\
	  ssize_t mid = (r - l) / 2;					\
	  								\
	  if ( x <= ecdf->x[ ecdf->index[mid] ] )			\
	    r = mid;							\
	  								\
	  if (l == mid)							\
	    mid++;							\
    									\
	  if ( x >= ecdf->x[ ecdf->index[mid] ] )			\
	    l = mid;							\
	}								\
    									\
    return (TYPE) (l+1) / (TYPE) ( ecdf->n + 1 );			\
  }									\
									\
  TYPE									\
  stats_ecdf_F_inv##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE p)	\
  {									\
    TYPE x = p * ecdf->n;						\
    size_t x0 = floor##SUFFIX(x);					\
    size_t x1 = ceil##SUFFIX(x);					\
    TYPE p0 = stats_ecdf_F##SUFFIX(ecdf, x0);				\
    TYPE p1 = stats_ecdf_F##SUFFIX(ecdf, x1);				\
    									\
    return ecdf->x[ x_index ] ;						\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
