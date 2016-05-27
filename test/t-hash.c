#include <stdlib.h>

#include "ale/error.h"
#include "ale/hash.h"

HASH_INIT(int, int , int, equal_func_int, hash_func_int)

int
main(int argc, char *argv[argc])
{
  struct hash_int hash;
  int ret, val;
  
  hash_int_init(&hash);
  ERROR_UNDEF_FATAL_FMT(HASH_DEFAULT_SIZE != hash.size, "FATAL: hash.size == %zu != %d\n", hash.size, HASH_DEFAULT_SIZE);

  ret = hash_int_set(&hash, 1, 1, NULL);
  ERROR_UNDEF_FATAL_FMT(0 != ret, "FATAL: hash_int_set(1) returned %d\n", ret);

  ERROR_UNDEF_FATAL_FMT(1 != (ret = hash_int_get(&hash, 1, &val)), "FATAL: hash_int_get(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: hash_int_get(1) value == %d != 1\n", val);

  
  hash_int_destroy(&hash);

  
  return EXIT_SUCCESS;
}
