#include <string.h>

#include "ale/algebra.h"
#include "ale/optimisation.h"
#include "ale/memory.h"
#include "ale/math.h"
#include "ale/error.h"


enum {
      ERROR_GRADIENT_DESCENT_TOO_MANY_ITER = 1
};

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  /* https://en.wikipedia.org/wiki/Broyden%E2%80%93Fletcher%E2%80%93Goldfarb%E2%80%93Shanno_algorithm */ \
  int optimisation_gradient_descent##SUFFIX(size_t n, TYPE x[n],	\
					    TYPE (*f)(TYPE *x, void *cls), \
					    void (*gradf)(TYPE *y, TYPE *x, void *cls), \
					    void *cls)			\
  {									\
    struct mem_pool pool;						\
    mem_init(&pool);							\
    int ret = 0;							\
    TYPE *p = mem_malloc(&pool, sizeof(TYPE) * n);			\
    TYPE *g = mem_malloc(&pool, sizeof(TYPE) * n);			\
    TYPE *delta_x = mem_malloc(&pool, sizeof(TYPE) * n);		\
    TYPE *delta_grad = mem_malloc(&pool, sizeof(TYPE) * n);		\
    TYPE alpha = 0.5;							\
    									\
    gradf(g, x, cls);							\
    									\
    for (size_t iter = 0 ;						\
	 alg_norm##SUFFIX(n, g) > ALE_EPS##SUFFIX ;			\
	 iter++)							\
      {									\
	alg_opposite_v##SUFFIX(n, g, p);				\
	alg_mul_v_c##SUFFIX(n, p, alpha, p);				\
									\
	memcpy(delta_x, x, sizeof(TYPE) * n);				\
	alg_add_v_v##SUFFIX(n, x, p, x);				\
	alg_sub_v_v##SUFFIX(n, x, delta_x, delta_x);			\
									\
	memcpy(delta_grad, g, sizeof(TYPE) * n);			\
	gradf(g, x, cls);						\
	alg_sub_v_v##SUFFIX(n, g, delta_grad, delta_grad);		\
	alpha = fabs##SUFFIX(alg_dot##SUFFIX(n, delta_x, delta_grad)) / alg_dot##SUFFIX(n, delta_grad, delta_grad); \
									\
	ret = (iter < 4096) ? 0 : -1;					\
	ERROR_CUSTOM_GOTO(-1 == ret, ERROR_GRADIENT_DESCENT_TOO_MANY_ITER, ERROR##SUFFIX); \
      }									\
									\
  ERROR##SUFFIX:							\
    mem_destroy(&pool);							\
    return ret;								\
  }



GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
