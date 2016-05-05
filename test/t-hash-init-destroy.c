#include <stdlib.h>

#include "ale/error.h"
#include "ale/hash.h"

int
main(int argc, char *argv[argc])
{
  struct hash hash;

  ERROR_UNDEF_FATAL(0 != hash_init(&hash), "FAIL: hash_init()\n" );
  ERROR_UNDEF_FATAL(hash.funcs != &hash_string_funcs, "FAIL: hash_init() funcs badly initialized\n");
  ERROR_UNDEF_FATAL_FMT(hash.size != HASH_DEFAULT_SIZE, 
  			"FAIL: hash_init() wrong size %zu != %zu\n", hash.size, (size_t) HASH_DEFAULT_SIZE);
  ERROR_UNDEF_FATAL(NULL == hash.hash_func_key, "FAIL: hash_init() hash_func is NULL\n");
  for (size_t i = 0 ; i < HASH_DEFAULT_SIZE ; i++)
    ERROR_UNDEF_FATAL(NULL != hash.array[i].next, "FAIL: hash_init() adjacency list unitialized\n");
 
  hash_destroy(&hash);

  ERROR_UNDEF_FATAL(0 != hash_init_full(&hash, HASH_DEFAULT_SIZE, &hash_intptr_t_funcs), "FAIL: hash_init()\n" );
  ERROR_UNDEF_FATAL(hash.funcs != &hash_intptr_t_funcs, "FAIL: hash_init() funcs badly initialized\n");

  hash_destroy(&hash);

  
  return EXIT_SUCCESS;
}
