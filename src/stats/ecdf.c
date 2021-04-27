#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/sort.h"


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
    ecdf->index = malloc( n * sizeof(size_t) );				\
    sort_q_indirect(ecdf->index, x, n, sizeof(TYPE),			\
		    sort_compar_double##SUFFIX, NULL);			\
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
    TYPE x0, x1;							\
    TYPE delta_p = (TYPE) 1 / (TYPE) (ecdf->n + 1);			\
    									\
    if ( x < ecdf->x[ ecdf->index[l] ])					\
      {									\
	x0 = ecdf->x[ ecdf->index[l] ];					\
	x1 = ecdf->x[ ecdf->index[l + 1] ];				\
      }									\
    else if ( x >= ecdf->x[ ecdf->index[r] ])				\
      {									\
	x0 = ecdf->x[ ecdf->index[r - 1] ];				\
	x1 = ecdf->x[ ecdf->index[r] ];					\
      }									\
    else								\
      {									\
	while (l < r)							\
	  {								\
	    ssize_t mid = (r - l) / 2;					\
	    								\
	    if ( x <= ecdf->x[ ecdf->index[mid] ] )			\
	      r = mid;							\
	    								\
	    if (l == mid)						\
	      mid++;							\
	    								\
	    if ( x >= ecdf->x[ ecdf->index[mid] ] )			\
	      l = mid;							\
	  }								\
	x0 = ecdf->x[ ecdf->index[l] ];					\
	x1 = ecdf->x[ ecdf->index[l + 1] ];				\
      }									\
    									\
    TYPE p0 = (TYPE) (l+1) * delta_p;					\
    return delta_p / (x1 - x0) * (x - x0) + p0;				\
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
    return 0 ;								\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
