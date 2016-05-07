#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/portability.h"

#define NUM_ITER (1l << 22)
#define DEVIATION (0.005)
#define BITS (8)
#define SIZE (1 << BITS)
#define SHIFT (32 - BITS)

int
main(int argc, char *argv[argc])
{
  uint32_t r;
  uint64_t sum[SIZE] = {0};
  double dsum[SIZE] = {0.0};
  
  for (unsigned long i = 0 ; i < NUM_ITER; i++)
    {
      portability_getrandom(&r, sizeof(r), 0);
      sum[r >> SHIFT] += 1;
    }

  const double exp = 1.0 /  (double) SIZE;
  for (unsigned long i = 0 ; i < SIZE ; i++)
    {
      dsum[i] = (double) sum[i] / (double) NUM_ITER;
      double diff = fabs(exp - dsum[i]);
      ERROR_UNDEF_FATAL_FMT(diff > DEVIATION, "%lu: |%f - %f| = %f\n", i, exp, dsum[i], diff);
    }

  return EXIT_SUCCESS;
}