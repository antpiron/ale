#include <stdlib.h>
#include <sys/timeb.h>

#include "ale/error.h"
#include "ale/portability.h"

#define MAX_INSERT (1 << 24)

int
main(int argc, char *argv[argc])
{
  uint64_t res = 0;
  double diff, rate;
  struct timeb st, et;


  printf("%s: ", argv[0]);
  
  ftime(&st);
  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      portability_getrandom(&res, sizeof(res), 0);
    }
  ftime(&et);
  diff = (et.time - st.time) + (et.millitm - st.millitm) / 1000.0;
  rate = MAX_INSERT / (diff * 1000 * 1000);
        
  printf("\nGenerated %12.2F MNumbers/sec, mean = %6.3f\n", rate, (double)res / MAX_INSERT);

  
  return EXIT_SUCCESS;
}
