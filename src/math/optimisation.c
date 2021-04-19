#include "ale/algebra.h"
#include "ale/optimisation.h"
#include "ale/memory.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  int optimisation_bfgs##SUFFIX(size_t n, TYPE x[n], TYPE direction,	\
				TYPE (*f)(TYPE *y, TYPE *x, void *cls), \
				TYPE (*gradf)(TYPE *y, TYPE *x, void *cls), \
				void *cls)				\
  {									\
    struct mem_pool pool;						\
    mem_init(&pool);							\
    TYPE (*B)[n] = mem_malloc(&pool, sizeof(*B) * n);			\
    TYPE *p = mem_malloc(&pool, sizeof(TYPE) * n);			\
    TYPE *g = mem_malloc(&pool, sizeof(TYPE) * n);			\
    									\
    alg_identity_init##SUFFIX(n, n, B);					\
    									\
    gradf(g, x, cls);							\
    alg_opposite_v##SUFFIX(n, g, g);					\
									\
    mem_destroy(&pool);							\
    return 0;								\
  }



GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
