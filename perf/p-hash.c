#include <stdlib.h>
#include <sys/timeb.h>

#include "ale/error.h"
#include "ale/hash.h"

#define MAX_INSERT (1 << 24)

HASH_INIT(int, int , int, equal_func_int, hash_func_int)

int
main(int argc, char *argv[argc])
{
  struct hash_int hash;
  int ret, val, key;
  double diff, rate;
  struct timeb st, et;

  printf("%s: ", argv[0]);
  
  hash_int_init_size(&hash, MAX_INSERT << 1);
  
  ftime(&st);
  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(0 != (ret = hash_int_set(&hash, i, i, &val)), "FATAL: hash_int_set(%d) returned %d\n", i, ret);
    }
  ftime(&et);
  diff = (et.time - st.time) + (et.millitm - st.millitm) / 1000.0;
  rate = MAX_INSERT / (diff * 1024 * 1024);
        
  printf("\nSet %12.2F MKeys/sec",rate);

  
  ftime(&st);
  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(1 != (ret = hash_int_get(&hash, i, &val)), "FATAL: hash_int_get(%d) returned %d\n", i, ret);
      ERROR_UNDEF_FATAL_FMT(i != val, "FATAL: hash_int_get(%d) value == %d != %d\n", i, val, i);
    }
  ftime(&et);
  diff = (et.time - st.time) + (et.millitm - st.millitm) / 1000.0;
  rate = MAX_INSERT / (diff * 1024 * 1024);
        
  printf("\nGet %12.2F MKeys/sec",rate);
  
  ftime(&st);
  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      ret = hash_int_delete(&hash, i, &key, &val);
      ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_int_delete(%d) returned %d\n", i, ret);
      ERROR_UNDEF_FATAL_FMT(i != key, "FATAL: hash_int_delete(%d) key == %d != %d\n", i, key, i);
      ERROR_UNDEF_FATAL_FMT(i != val, "FATAL: hash_int_delete(%d) value == %d != %d\n", i, val, i);
    }
  ftime(&et);
  diff = (et.time - st.time) + (et.millitm - st.millitm) / 1000.0;
  rate = MAX_INSERT / (diff * 1024 * 1024);
        
  printf("\nDel %12.2F MKeys/sec",rate);

  printf("\n");
  
  hash_int_destroy(&hash);

  
  return EXIT_SUCCESS;
}
