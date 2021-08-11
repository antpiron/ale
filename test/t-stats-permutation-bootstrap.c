#define _GNU_SOURCE
#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/sort.h"
#include "ale/math.h"
#include <math.h>

#define ORDER 16
#define EPS (0.0000000001d)

double
predict_no_ld(size_t i, double x, void *cls)
{
  return -1;
}


int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN], res[LEN];
  ssize_t deps[LEN];
  int ret;
  struct stats_permutation permutation;

  for (size_t i = 0 ; i < LEN ; i++) 
    x[i] = i;

  stats_permutation_init(&permutation, LEN, x);
  // ERROR_FATAL(0 != ret, "FAIL: stats_permutation_init() != 0\n");
  
  for (size_t i = 0 ; i < LEN ; i++) 
    deps[i] = -1;


  stats_permutation_dependencies(&permutation, deps, predict_no_ld, NULL);

  stats_permutation(&permutation, res);

  stats_permutation_destroy(&permutation);
 

   
  return EXIT_SUCCESS;
}
