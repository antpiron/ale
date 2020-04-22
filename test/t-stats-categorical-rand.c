#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (4)
  double cumul_p[LEN] = {0.1, 0.4, 0.9, 1.0};
  size_t cumul[LEN] = {0};

  for (int i = 0 ; i < LEN ; i++)
    {
      size_t k = stats_categorical_rand(LEN, cumul_p);
      ERROR_UNDEF_FATAL_FMT(k > LEN, "FAIL: stats_categorical_rand() = %zu >= %zu \n", k, LEN);
      printf("\n");
    }
  

  return EXIT_SUCCESS;
}
