#include <string.h>

#include "ale/algebra.h"
#include "ale/optimisation.h"
#include "ale/memory.h"
#include "ale/math.h"
#include "ale/error.h"


#define MAX_ITER (1ll << 12)

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  struct interpolation_linear##SUFFIX					\
  {									\
    size_t n;								\
    TYPE *x;								\
    TYPE *y;								\
  };									\
  									\
  void									\
  interpolation_init##SUFFIX(struct interpolation_linear##SUFFIX *inter, \
			     size_t n, TYPE y[n], TYPE x[n])		\
  {									\
    inter->n = n;							\
    inter->y = y;							\
    inter->x = x;							\
  }									\
									\
  struct interpolation_min_max##SUFFIX					\
  {									\
    TYPE min, max;							\
  };									\
									\
  TYPE									\
  interpolation_out_of_domain_linear##SUFFIX(struct interpolation_linear##SUFFIX *inter, TYPE x, void *cls) \
  {									\
    struct interpolation_min_max##SUFFIX *mm = cls;			\
    size_t l = 0, r = inter->n;						\
    TYPE x0, y0, x1, y1;						\
    TYPE result;							\
    									\
    if ( x < inter->x[0] )						\
      {									\
	l = 0;								\
	r = 1;								\
      }									\
    else if ( x >= inter->x[inter->n - 1] )				\
      {									\
	l = inter->n - 2;						\
	r = inter->n - 1;						\
      }									\
    									\
    x0 = inter->x[l];							\
    y0 = inter->y[l];							\
    x1 = inter->x[r];							\
    y1 = inter->y[r];							\
    									\
    result = (y1 - y0) / (x1 - x0) * (x - x0) + y0;			\
									\
    if ( NULL != mm )							\
      {									\
	if (result < mm->min)						\
	  result = mm->min;						\
									\
	if (result > mm->max)						\
	  result = mm->max;						\
      }									\
									\
    return result;							\
  }									\
									\
  TYPE									\
  interpolation_linear_f##SUFFIX(struct interpolation_linear##SUFFIX *inter, \
				 TYPE x,				\
				 TYPE (*out_of_domain)(struct interpolation_linear##SUFFIX *inter, TYPE x, void *cls), \
				 void *cls)				\
  {									\
    size_t l = 0, r = inter->n;						\
    TYPE x0, y0, x1, y1;						\
    									\
    if ( x < inter->x[0] || x >= inter->x[inter->n - 1] )		\
      {									\
	if (NULL == out_of_domain)					\
	  return interpolation_out_of_domain_linear##SUFFIX(inter, x, cls); \
									\
	return out_of_domain(inter, x, cls);				\
      }									\
									\
    while (1)								\
      {									\
	size_t mid = (l + r) / 2;					\
	    								\
	if ( x < inter->x[mid] )					\
	  r = mid;							\
	else if (l != mid)						\
	  l = mid;							\
	else								\
	  break;							\
      }									\
    									\
    x0 = inter->x[l];							\
    y0 = inter->y[l];							\
    x1 = inter->x[r];							\
    y1 = inter->y[r];							\
    									\
    return (y1 - y0) / (x1 - x0) * (x - x0) + y0;			\
  }


GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
