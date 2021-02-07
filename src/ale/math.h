#ifndef __ALE_MATH_H
#define __ALE_MATH_H

#include <math.h>
#include <float.h>

#define ALE_EPS (DBL_MIN)
#define ALE_EPSl (LDBL_MIN)

#define ALE_TINY_DOUBLE (DBL_MIN)
#define ALE_TINY_DOUBLEl (LDBL_MIN)


#define MATH_GENERIC_HEADERS(SUFFIX,TYPE)				\
  static inline TYPE ale_beta##SUFFIX(TYPE a, TYPE b);			\
  static inline TYPE ale_lbeta##SUFFIX(TYPE a, TYPE b);			\
  static inline TYPE ale_lgamma##SUFFIX(TYPE x);			\
  static inline TYPE ale_gamma##SUFFIX(TYPE x);				\
  TYPE ale_rilgamma##SUFFIX(TYPE x, TYPE a);				\
  TYPE ale_riugamma##SUFFIX(TYPE x, TYPE a);				\
  TYPE ale_ibeta##SUFFIX(TYPE x, TYPE a, TYPE b);			\
  static inline int ale_cmp_double##SUFFIX(TYPE a, TYPE b, TYPE eps);


MATH_GENERIC_HEADERS(,double)
MATH_GENERIC_HEADERS(l,long double)


// Inline body
#define MATH_GENERIC_FUNC(SUFFIX,TYPE)					\
  static inline								\
  int									\
  ale_cmp_double##SUFFIX(TYPE a, TYPE b, TYPE eps)			\
  {									\
    TYPE diff = a - b;							\
    TYPE max = (fabs##SUFFIX(a) >= fabs##SUFFIX(b)) ?			\
      fabs##SUFFIX(a) :							\
      fabs##SUFFIX(b);							\
    									\
    if ( fabs##SUFFIX(diff) <= max * eps )				\
      return 0;								\
    									\
    if ( diff < 0.0 )							\
      return -1;							\
    									\
    return 1;								\
  }									\
									\
  static inline								\
  TYPE									\
  ale_gamma##SUFFIX(TYPE x)						\
  {									\
    return tgamma##SUFFIX(x);						\
  }									\
									\
  static inline								\
  TYPE									\
  ale_lgamma##SUFFIX(TYPE x)						\
  {									\
    return lgamma##SUFFIX(x);						\
  }									\
									\
  static inline								\
  TYPE									\
  ale_beta##SUFFIX(TYPE a, TYPE b)					\
  {									\
    return exp(ale_lbeta##SUFFIX(a, b));				\
  }									\
									\
  static inline								\
  TYPE									\
  ale_lbeta##SUFFIX(TYPE a, TYPE b)					\
  {									\
    return ale_lgamma##SUFFIX(a) + ale_lgamma##SUFFIX(b) -		\
      ale_lgamma##SUFFIX(a + b);					\
  }									

MATH_GENERIC_FUNC(,double)
MATH_GENERIC_FUNC(l,long double)




#endif
