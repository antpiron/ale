#define _GNU_SOURCE
#include <stdlib.h>

#include "ale/sort.h"

struct indirect_closure
{
  size_t nmemb;
  void *base;
  void *arg;
  int (*compar)(const void *, const void *, void *);
};

inline int
sort_compar_double(const void *p1, const void *p2, void *arg)
{
  const double *a = p1;
  const double *b = p2;
  // disable unused parameter warning
  (void)(arg);

  if (*a < *b)
    return -1;
  
  if (*a > *b)
    return  1;
  
  return 0;
}

int
sort_compar_double_decreasing(const void *p1, const void *p2, void *arg)
{
  return - sort_compar_double(p1, p2, arg);
}

static int
indirect_compar(const void *p1, const void *p2, void *arg)
{
  struct indirect_closure *closure = arg;
  const size_t *a = p1;
  const size_t *b = p2;
  const char (*base)[closure->nmemb] = closure->base;


  return closure->compar(base + *a , base + *b, closure->arg);
}

void
sort_q_indirect(size_t *index, void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *),
		void *arg)
{
  struct indirect_closure closure = {.nmemb = nmemb, .base = base, .arg = arg, .compar = compar};
  
  for (size_t i = 0 ; i < size ; i++)
    index[i] = i;
  
  qsort_r(index, size, sizeof(index[0]),
          indirect_compar, (void*) &closure);

}
