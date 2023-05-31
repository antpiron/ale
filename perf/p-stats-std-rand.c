#include <stdlib.h>
#include <time.h>

#include "ale/error.h"
#include "ale/stats.h"
#include <omp.h>

#define MAX_INSERT (1 << 23)

int
main(int argc, char *argv[argc])
{
  double res = 0;
  double diff, rate;
  struct timespec st, et;
  size_t n = MAX_INSERT;


  printf("%s: ", argv[0]);
  
  clock_gettime(CLOCK_MONOTONIC, &st);
  for (size_t i = 0 ; i < n ; i++)
    {
      res += stats_unif_std_rand();
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = n / (diff * 1000 * 1000);
        
  printf("\nGenerated %12.2F MNumbers/sec, mean = %6.3f\n", rate, res / MAX_INSERT);

  clock_gettime(CLOCK_MONOTONIC, &st);
  res = 0;
  int n_threads = omp_get_max_threads();
  n = MAX_INSERT * n_threads;
#pragma omp parallel for reduction (+:res)
  for (size_t i = 0 ; i < n ; i++)
    {
      res += stats_unif_std_rand();
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = n / (diff * 1000 * 1000);
        
  printf("Generated %12.2F MNumbers/sec in parallel (n = %d), by threads %.2F MNumbers/sec, mean = %6.3f\n", rate, n_threads, rate / n_threads, res / n);

  
  
  return EXIT_SUCCESS;
}
