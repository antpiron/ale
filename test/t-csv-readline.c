#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/fileutils.h"

#define CSV "csv-test.csv"

int
main(int argc, char *argv[argc])
{
  struct csv csv;
  struct vector_int vector;
  FILE *file;
  size_t ret;
  char *exp2 = "plop\"plop2,\nnewline";

  vector_int_init(&vector);
  
  file = fopen(CSV, "r");
  ERROR_ERRNO_FATAL(NULL == file, "FATAL: fopen() returned NULL\n");

  ERROR_UNDEF_FATAL( 0 != csv_init(&csv, file), "FATAL: csv_init() != 0\n");

  ERROR_UNDEF_FATAL_FMT( 3 != (ret = csv_readline(&csv, &vector)), "FATAL: returned %zu\n", ret);

  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("plop", vector.data[0]), "FATAL: %s != plop\n", vector.data[0]);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp(exp2, vector.data[1]), "FATAL: %s != %s\n", vector.data[1], exp2);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("kgfgf",  vector.data[2]), "FATAL: %s != kgfgf\n",  vector.data[2]);

  for (size_t i = 0 ; i < 3 ; i++)
    free(vector.data[i]);
  vector_int_destroy(&vector);

  
  vector_int_init(&vector);

  ERROR_UNDEF_FATAL_FMT( 3 != (ret = csv_readline(&csv, &vector)), "FATAL: returned %zu\n", ret);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("one", vector.data[0]), "FATAL: %s != one\n", vector.data[0]);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("two", vector.data[1]), "FATAL: %s != two\n", vector.data[1]);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("three",  vector.data[2]),"FATAL: %s != three\n", (char*) vector.data[2]);

  ERROR_UNDEF_FATAL_FMT( 0 != (ret = csv_readline(&csv, &vector)), "FATAL: returned %zu\n", ret);

  for (size_t i = 0 ; i < 3 ; i++)
    free(vector.data[i]);
  vector_int_destroy(&vector);
 
  csv_destroy(&csv);
  fclose(file);
  
  return EXIT_SUCCESS;
}
