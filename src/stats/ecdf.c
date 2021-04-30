#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/sort.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
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
    ssize_t l = 0, r = ecdf->n;						\
    									\
    if ( x < ecdf->x[ ecdf->index[0] ])					\
      return 0;								\
    									\
    if ( x >= ecdf->x[ ecdf->index[r - 1] ])				\
      return 1;								\
    									\
    while (1)								\
      {									\
	ssize_t mid = (r + l) / 2;					\
									\
	if ( x < ecdf->x[ ecdf->index[mid] ] )				\
	  r = mid;							\
	else if ( l != mid )						\
	  l = mid;							\
	else								\
	  break;							\
      }									\
    									\
    return (double) (l+1) / ecdf->n;					\
  }									\
									\
  TYPE									\
  stats_ecdf_F_inv##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE p)	\
  {									\
    									\
    									\
    return 0 ;								\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
