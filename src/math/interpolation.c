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
    size_t i = 0;							\
    inter->n = n;							\
  }									\
  									\
  TYPE									\
  interpolation_f##SUFFIX(struct interpolation_linear##SUFFIX *inter, TYPE x) \
  {									\
    return 0;								\
  }


GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
