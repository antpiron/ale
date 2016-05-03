#include <stdlib.h>

#include "ale/error.h"
#include "ale/hash.h"

int
main(int argc, char *argv[argc])
{
  struct hash hash;
  intptr_t val;
  
  ERROR_UNDEF_FATAL(0 != hash_init_full(&hash, HASH_DEFAULT_SIZE, &hash_intptr_t_funcs),
		    "FAIL: hash_init()\n" );

  ERROR_UNDEF_FATAL(NULL != hash_set(&hash, (void*)(intptr_t) 1, 0, (void*)(intptr_t) 1),
		    "FAIL: hash_set() returned not NULL\n");
  ERROR_UNDEF_FATAL(NULL == hash.array[1].next,
		    "FAIL: hash_set() mmh seems like nothing was inserted\n");
  val = (intptr_t) hash_get(&hash, (void*)(intptr_t) 1, 0);
  ERROR_UNDEF_FATAL(1 != val,
		    "FAIL: hash_get() mmh seems like nothing was inserted\n");
  
  ERROR_UNDEF_FATAL((void*)(intptr_t) 1 != hash_set(&hash, (void*)(intptr_t) 1, 0, (void*)(intptr_t) 1),
		    "FAIL: hash_set() returned NULL\n");
  hash_destroy(&hash);

  ERROR_UNDEF_FATAL(0 != hash_init_full(&hash, HASH_DEFAULT_SIZE, &hash_intptr_t_funcs),
		    "FAIL: hash_init()\n" );

  for (intptr_t i = 0 ; i < HASH_DEFAULT_SIZE ; i++)
    {
      ERROR_UNDEF_FATAL(NULL != hash_set(&hash, (void*)(intptr_t) i, 0, (void*)(intptr_t) (i*2)),
			"FAIL: hash_set() returned not NULL\n");
      ERROR_UNDEF_FATAL(NULL == hash.array[i].next,
			"FAIL: hash_set() mmh seems like nothing was inserted\n");
    }

  for (intptr_t i = 0 ; i < HASH_DEFAULT_SIZE ; i++)
    {
      val = (intptr_t) hash_get(&hash, (void*)(intptr_t) i, 0);
      ERROR_UNDEF_FATAL_FMT(val != i*2, "FAIL: %"PRIdPTR"!= %"PRIdPTR"\n", val, i*2);
    }

  for (intptr_t i = HASH_DEFAULT_SIZE ; i < 2*HASH_DEFAULT_SIZE ; i++)
    {
      ERROR_UNDEF_FATAL(NULL != hash_set(&hash, (void*)(intptr_t) i, 0, (void*)(intptr_t) (i*2)),
			"FAIL: hash_set() returned not NULL\n");
    }

  for (intptr_t i = HASH_DEFAULT_SIZE ; i < 2*HASH_DEFAULT_SIZE ; i++)
    {
      val = (intptr_t) hash_get(&hash, (void*)(intptr_t) i, 0);
      ERROR_UNDEF_FATAL_FMT(val != i*2, "FAIL: %"PRIdPTR"!= %"PRIdPTR"\n", val, i*2);
    }
  
  hash_destroy(&hash);
  
  return EXIT_SUCCESS;
}
