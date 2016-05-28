#include <stdlib.h>

#include "ale/error.h"
#include "ale/hash.h"

#define MAX_INSERT (1 << 18)

HASH_INIT(int, int , int, equal_func_int, hash_func_int)

int
main(int argc, char *argv[argc])
{
  struct hash_int hash;
  int ret, val, key;
  
  hash_int_init(&hash);
  ERROR_UNDEF_FATAL_FMT(HASH_DEFAULT_SIZE != hash.size, "FATAL: hash.size == %zu != %d\n", hash.size, HASH_DEFAULT_SIZE);

  ret = hash_int_set(&hash, 1, 1, NULL);
  ERROR_UNDEF_FATAL_FMT(0 != ret, "FATAL: hash_int_set(1) returned %d\n", ret);

  ERROR_UNDEF_FATAL_FMT(1 != (ret = hash_int_get(&hash, 1, &val)), "FATAL: hash_int_get(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: hash_int_get(1) value == %d != 1\n", val);

  val = 0;
  ret = hash_int_set(&hash, 1, 1, &val);
  ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_int_set(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: hash_int_set(1) value == %d != 1\n", val);

  ret = hash_int_delete(&hash, 1, &key, &val);
  ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_int_delete(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != key, "FATAL: hash_int_delete(1) key == %d != 1\n", key);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: hash_int_delete(1) value == %d != 1\n", val);

  ERROR_UNDEF_FATAL_FMT(0 != (ret = hash_int_delete(&hash, 1, &key, &val)),
			"FATAL: hash_int_delete(1) returned %d\n", ret);

  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(0 != (ret = hash_int_set(&hash, i, i, &val)), "FATAL: hash_int_set(%d) returned %d\n", i, ret);
    }

  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(1 != (ret = hash_int_get(&hash, i, &val)), "FATAL: hash_int_get(%d) returned %d\n", i, ret);
      ERROR_UNDEF_FATAL_FMT(i != val, "FATAL: hash_int_get(%d) value == %d != %d\n", i, val, i);
    }
  
  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      ret = hash_int_delete(&hash, i, &key, &val);
      ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_int_delete(%d) returned %d\n", i, ret);
      ERROR_UNDEF_FATAL_FMT(i != key, "FATAL: hash_int_delete(%d) key == %d != %d\n", i, key, i);
      ERROR_UNDEF_FATAL_FMT(i != val, "FATAL: hash_int_delete(%d) value == %d != %d\n", i, val, i);
    }
  
  hash_int_destroy(&hash);

  
  return EXIT_SUCCESS;
}