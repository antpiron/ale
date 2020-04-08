#include <stdlib.h>

#include <string.h>

#include "ale/error.h"
#include "ale/index.h"


int
main(int argc, char *argv[argc])
{
  struct index index;
  int ret;
  ssize_t res;
  
  index_init(&index);
  ERROR_UNDEF_FATAL_FMT(-1 != index.max, "FAIL: index_init() max = %zu != -1\n",  index.max);

  ret = index_set(&index, "Plop!", 0);
  ERROR_FATAL(-1 == ret, "FAIL: index_set() ret == -1\n");
  ERROR_UNDEF_FATAL(1 == ret, "FAIL: index_set() ret == 1\n");

  char *str = index_rget(&index, 0);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(str, "Plop!"),
			"FAIL: index_rget() key = %s\n", str);
  
  res = index_get(&index, "Plop!");
  ERROR_UNDEF_FATAL(-1 == res, "FAIL: index_get() res = -1\n");
  ERROR_UNDEF_FATAL_FMT(0 != res, "FAIL: index_set() res = %z != 0\n", res);

  res = index_get(&index, "Nawak!");
  ERROR_UNDEF_FATAL_FMT(-1 != res, "FAIL: index_get(Nawak!) res = %z != -1\n", res);

  index_destroy(&index);
  
  return EXIT_SUCCESS;
}
