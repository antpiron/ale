#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>

#include "ale/error.h"
#include "ale/portability.h"

// qsort_r in POSIX 2024 order, redirected by portability.h on BSD/macOS

#define LEN 64

struct closure
{
  const double *keys;
  size_t ncalls;
  int bad_arg;
};

static struct closure *expected_closure;

static int
compar_indirect(const void *p1, const void *p2, void *arg)
{
  struct closure *closure = arg;
  const size_t *a = p1;
  const size_t *b = p2;

  if (closure != expected_closure)
    {
      // wrong argument order
      expected_closure->bad_arg = 1;
      return 0;
    }

  closure->ncalls++;

  if (closure->keys[*a] < closure->keys[*b])
    return -1;

  if (closure->keys[*a] > closure->keys[*b])
    return 1;

  return 0;
}

int
main(int argc, char *argv[argc])
{
  double keys[LEN];
  size_t index[LEN];
  struct closure closure = {.keys = keys};

  expected_closure = &closure;

  // distinct keys in scrambled order: 37 is coprime with LEN
  for (size_t i = 0 ; i < LEN ; i++)
    {
      keys[i] = (double) ((i * 37) % LEN);
      index[i] = i;
    }

  qsort_r(index, LEN, sizeof(index[0]), compar_indirect, &closure);

  ERROR_UNDEF_FATAL(closure.bad_arg, "FAIL: compar received a wrong arg pointer\n");
  ERROR_UNDEF_FATAL(0 == closure.ncalls, "FAIL: compar never called\n");

  for (size_t i = 0 ; i < LEN ; i++)
    ERROR_UNDEF_FATAL_FMT(keys[index[i]] != (double) i,
			  "FAIL: keys[index[%zu]] == %f != %zu\n", i, keys[index[i]], i);

  closure.ncalls = 0;
  qsort_r(index, 0, sizeof(index[0]), compar_indirect, &closure);
  qsort_r(index, 1, sizeof(index[0]), compar_indirect, &closure);
  ERROR_UNDEF_FATAL_FMT(0 != closure.ncalls, "FAIL: compar called %zu times on nmemb <= 1\n", closure.ncalls);

  return EXIT_SUCCESS;
}
