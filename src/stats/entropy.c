#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/sort.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  stats_entropy##SUFFIX(size_t n, TYPE x[n])				\
  {									\
    struct stats_kd##SUFFIX *kd;					\
    TYPE sum = 0;							\
									\
    return sum / n;							\
  }


GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
