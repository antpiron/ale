#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/vector.h"


VECTOR_INIT(int,int)

int
main(int argc, char *argv[argc])
{
  struct vector_int vector;
  int val;


  vector_int_init(&vector);
  ERROR_UNDEF_FATAL_FMT(vector.size != VECTOR_DEFAULT_SIZE, "FATAL: vector_int_init() size %zu != %d\n", vector.size, VECTOR_DEFAULT_SIZE);

  vector_int_set(&vector, 1, 1);
  ERROR_UNDEF_FATAL_FMT(vector.data[1] != 1, "FATAL: vector_int_set() size %d != 1\n", vector.data[1]);

  vector_int_set(&vector, 512, 512);
  ERROR_UNDEF_FATAL_FMT(vector.data[512] != 512, "FATAL: vector_int_set() size %d != 512\n", vector.data[512]);

  val = vector_int_get(&vector, 1);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: vector_int_get() size %d != 1\n", val);
  val = vector_int_get(&vector, 512);
  ERROR_UNDEF_FATAL_FMT(512 != val, "FATAL: vector_int_get() size %d != 512\n", val);
  
  vector_int_destroy(&vector);
  
  return EXIT_SUCCESS;
}
