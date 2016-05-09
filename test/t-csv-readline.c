#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/fileutils.h"
#include "ale/sl_list.h"

#define CSV "csv-test.csv"

int
main(int argc, char *argv[argc])
{
  struct sl_node node;
  FILE *file;
  size_t ret;
  char *exp2 = "plop\"plop2,\nnewline";

  sl_init(&node);
  
  file = fopen(CSV, "r");
  ERROR_ERRNO_FATAL(NULL == file, "FATAL: fopen() returned NULL\n");

  ERROR_UNDEF_FATAL( 0 != csv_init(file), "FATAL: csv_init() != 0\n");

  ERROR_UNDEF_FATAL_FMT( 3 != (ret = csv_readline(&node)), "FATAL: returned %zu\n", ret);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("plop", (char*) node.next->data),
			 "FATAL: %s != plop\n", (char*) node.next->data);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp(exp2, (char*) node.next->next->data),
			 "FATAL: %s != %s\n", (char*) node.next->next->data, exp2);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("kgfgf", (char*) node.next->next->next->data),
			 "FATAL: %s != kgfgf\n", (char*) node.next->next->next->data);

  sl_destroy_full(&node, free);

  sl_init(&node);

  ERROR_UNDEF_FATAL_FMT( 3 != (ret = csv_readline(&node)), "FATAL: returned %zu\n", ret);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("one", (char*) node.next->data),
			 "FATAL: %s != one\n", (char*) node.next->data);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("two", (char*) node.next->next->data),
			 "FATAL: %s != two\n", (char*) node.next->next->data);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("three", (char*) node.next->next->next->data),
			 "FATAL: %s != three\n", (char*) node.next->next->next->data);

  ERROR_UNDEF_FATAL_FMT( 0 != (ret = csv_readline(&node)), "FATAL: returned %zu\n", ret);
  sl_destroy_full(&node, free);


  fclose(file);
  
  return EXIT_SUCCESS;
}
