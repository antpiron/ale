#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/stringutils.h"

int
main(int argc, char *argv[argc])
{
  struct string str1, str2;

  string_init(&str1);
  string_set(&str1, "Hello");
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(str1.str, "Hello"), "FATAL: %s != 'Hello!'\n", str1.str);
  ERROR_UNDEF_FATAL_FMT(str1.len != 5, "FATAL: %zu != 5'\n", str1.len);

  string_init(&str2);
  string_set(&str2, " Plop!");
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(str2.str, " Plop!"), "FATAL: %s != ' Plop!'\n", str2.str);
  ERROR_UNDEF_FATAL_FMT(str2.len != 6, "FATAL: %zu != 6'\n", str1.len);
 
  string_append(&str1, &str2);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(str1.str, "Hello Plop!"), "FATAL: %s != 'Hello Plop!'\n", str1.str);
  ERROR_UNDEF_FATAL_FMT(str1.len != 11, "FATAL: %zu != 11'\n", str1.len);

  string_destroy(&str1);
  string_destroy(&str2);
  
  return EXIT_SUCCESS;
}
