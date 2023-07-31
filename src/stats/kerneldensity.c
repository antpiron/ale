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
			     TYPE (*K)(TYPE))				\
  {									\
    kd->n= n;								\
    kd->x = x;								\
    kd->h = h;								\
    kd->K = K;								\
    /* mem_init(&kd->pool); */						\
    /* kd->index = mem_malloc(&kd->pool, n * sizeof(size_t) ); */	\
									\
    /* sort_q_indirect(kd->index, x, n, sizeof(TYPE), */		\
    /* sort_compar_double##SUFFIX, NULL); */				\
  }									\
									\
  void									\
  stats_kd_init##SUFFIX(struct stats_kd##SUFFIX *kd, size_t n,		\
		TYPE x[n])						\
  {									\
    /* TODO: h computation*/						\
    stats_kd_init_full##SUFFIX(kd, n, x, 1, &stats_norm_std_f##SUFFIX);	\
  }									\
  void									\
  stats_kd_destroy##SUFFIX(struct stats_kd##SUFFIX *kd)			\
  {									\
    /* mem_destroy(&kd->pool); */					\
  }									\
  									\
  TYPE									\
  stats_kd_f##SUFFIX(struct stats_kd##SUFFIX *kd, TYPE x)		\
  {									\
    TYPE sum = 0;							\
									\
    for (size_t i = 0 ; i < kd->n ; i++)				\
      sum += kd->K((x - kd->x[i]) / kd->h);				\
									\
    return 1 / (kd->n * kd->h) * sum;					\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
