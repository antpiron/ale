#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include "ale/error.h"
#include "csv_tokens.h"

int
main(int argc, char *argv[argc])
{
  char *str = strdup("\"plop\"\" \"\" plop2\"");
  char *exp = "plop\" \" plop2";

  str = clean_escaped(str);

  ERROR_FATAL_FMT(0 != strcmp(str, exp), "FAIL: clean_escaped() %s != %s\n", str, exp);

  free(str);
  
  return EXIT_SUCCESS;
}
