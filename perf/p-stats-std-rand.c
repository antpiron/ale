#include <stdlib.h>
#include <sys/timeb.h>

#include "ale/error.h"
#include "ale/stats.h"

#define MAX_INSERT (1 << 22)

int
main(int argc, char *argv[argc])
{
  double res = 0;
  double diff, rate;
  struct timeb st, et;


  printf("%s: ", argv[0]);
  
  ftime(&st);
  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      res += stats_unif_std_rand();
    }
  ftime(&et);
  diff = (et.time - st.time) + (et.millitm - st.millitm) / 1000.0;
  rate = MAX_INSERT / (diff * 1000 * 1000);
        
  printf("\nGenerated %12.2F MNumbers/sec, mean = %6.3f\n", rate, res / MAX_INSERT);

  
  return EXIT_SUCCESS;
}
