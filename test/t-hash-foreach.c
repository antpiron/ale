#include <stdlib.h>

#include "ale/error.h"
#include "ale/hash.h"


// generate come collision
#define NUMBER (1 << 18)

void
callback(struct hash_kv *kv, void *cls)
{
  unsigned long *count = cls;

  (*count)++;
}
  
int
main(int argc, char *argv[argc])
{
  struct hash hash;
  unsigned long count = 0;

  ERROR_UNDEF_FATAL(0 != hash_init_full(&hash, HASH_DEFAULT_SIZE, &hash_intptr_t_funcs),
  		    "FAIL: hash_init_full()\n" );

  for (intptr_t i = 0 ; i <  NUMBER ; i++)
    {
      ERROR_UNDEF_FATAL(-1 == hash_set(&hash, (void*)(intptr_t) i, 0, (void*)(intptr_t) i, NULL),
  			"FAIL: hash_set() returned -1\n");
    }

  hash_foreach(&hash, callback, &count);
  ERROR_UNDEF_FATAL_FMT(count != NUMBER, "FATAL: %lu != %lu\n", count, NUMBER);
  
  hash_destroy(&hash);

  return EXIT_SUCCESS;
}
