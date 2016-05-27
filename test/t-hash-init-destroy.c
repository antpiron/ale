#include <stdlib.h>

#include "ale/error.h"
#include "ale/hash.h"

HASH_INIT(int, int , int, equal_func_int, hash_func_int)

int
main(int argc, char *argv[argc])
{
  struct hash_int hash;

  hash_int_init(&hash);
  
  hash_int_destroy(&hash);

  
  return EXIT_SUCCESS;
}
