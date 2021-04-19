#ifndef __ALE_OPTIMISATION_H
#define __ALE_OPTIMISATION_H

#define OPTIMISATION_GENERIC_HEADERS(SUFFIX,TYPE)			\
  int optimisation_gradient_descent##SUFFIX(size_t n, TYPE x[n],	\
					    TYPE (*f)(TYPE *x, void *cls), \
					    void (*gradf)(TYPE *y, TYPE *x, void *cls), \
					    void *cls);


OPTIMISATION_GENERIC_HEADERS(,double)
OPTIMISATION_GENERIC_HEADERS(l,long double)


#endif
