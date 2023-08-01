#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/sort.h"
#include "ale/interpolation.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  void									\
  stats_kd_init_full##SUFFIX(struct stats_kd##SUFFIX *kd, size_t n,	\
			     TYPE x[n], TYPE h,				\
			     TYPE (*K)(TYPE),				\
			     TYPE K_eps)				\
  {									\
    kd->n= n;								\
    kd->x = x;								\
    kd->h = h;								\
    kd->K = K;								\
    kd->K_eps = K_eps;							\
    mem_init(&kd->pool);						\
    kd->index = mem_malloc(&kd->pool, n * sizeof(size_t) );		\
									\
    sort_q_indirect(kd->index, x, n, sizeof(TYPE),			\
		    sort_compar_double##SUFFIX, NULL);			\
  }									\
									\
  void									\
  stats_kd_init##SUFFIX(struct stats_kd##SUFFIX *kd, size_t n,		\
			TYPE x[n])					\
  {									\
    /* https://en.wikipedia.org/wiki/Kernel_density_estimation#A_rule-of-thumb_bandwidth_estimator */ \
    TYPE IQR = stats_IQR##SUFFIX(n, x);					\
    TYPE sd = stats_sd##SUFFIX(n, x);					\
    TYPE A = fmin##SUFFIX(sd, IQR / 1.23);				\
    TYPE h = 0.9 * A * pow##SUFFIX((TYPE) n, -0.2);			\
									\
    stats_kd_init_full##SUFFIX(kd, n, x, h,				\
			       &stats_norm_std_f##SUFFIX,		\
			       0.001);					\
  }									\
  void									\
  stats_kd_destroy##SUFFIX(struct stats_kd##SUFFIX *kd)			\
  {									\
    mem_destroy(&kd->pool);						\
  }									\
  									\
  TYPE									\
  stats_kd_f##SUFFIX(struct stats_kd##SUFFIX *kd, TYPE x)		\
  {									\
    TYPE sum = 0;							\
    ssize_t below = 0, above = kd->n - 1;				\
									\
    while ( below < above - 1)						\
      {									\
	size_t half = (below + above) / 2;				\
	TYPE val = kd->x[kd->index[half]];				\
									\
	if ( x < val  )							\
	  above = half;							\
	else if (x > val)						\
	  below = half;							\
	else								\
	  { below = half ; above = half + 1;}				\
									\
      }									\
									\
    for (size_t i = above ; i < kd->n ; i++)				\
      {									\
	TYPE param_x = (x - kd->x[kd->index[i]] ) / kd->h;		\
	TYPE K_val =  kd->K(param_x);					\
									\
	sum += K_val;							\
									\
	if (fabs##SUFFIX(K_val) < kd->K_eps)				\
	  break;							\
      }									\
									\
    for (ssize_t i = below ; i >= 0 ; i--)				\
      {									\
	TYPE param_x = (x - kd->x[kd->index[i]] ) / kd->h;		\
	TYPE K_val =  kd->K(param_x);					\
									\
	sum += K_val;							\
									\
	if (fabs##SUFFIX(K_val) < kd->K_eps)				\
	  break;							\
      }									\
									\
    return 1.0 / (kd->n * kd->h) * sum;					\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
