#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/portability.h"

#define NUM_ITER (1l << 20)

int
main(int argc, char *argv[argc])
{
  unsigned int r;
  
  for (unsigned long i = 0 ; i < NUM_ITER; i++)
    {
      portability_getrandom(&r, sizeof(r), 0);
      printf("%u\n", r);
    }

  return EXIT_SUCCESS;
}
