#ifndef __ALE_OPTIMISATION_H
#define _ALE_OPTIMISATION_H

#define MATH_GENERIC_HEADERS(SUFFIX,TYPE)				\
  int optimisation_bfgs##SUFFIX(size_t n, TYPE x[n], TYPE direction,	\
				TYPE (*f)(TYPE *y, TYPE *x, void *cls), \
				TYPE (*gradf)(TYPE *y, TYPE *x, void *cls), \
				void *cls);


MATH_GENERIC_HEADERS(,double)
MATH_GENERIC_HEADERS(l,long double)


#endif
