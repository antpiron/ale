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

  struct string *strp1 = string_new("Plop!");
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(strp1->str, "Plop!"), "FATAL: %s != 'Plop!'\n", strp1->str);
  struct string *strp2 = string_new("Hello");
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(strp2->str, "Hello"), "FATAL: %s != 'Hello'\n", strp2->str);

  string_append_c(strp2, " ");
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(strp2->str, "Hello "), "FATAL: %s != 'Hello '\n", strp2->str);
  string_append(strp2, strp1);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(strp2->str, "Hello Plop!"), "FATAL: %s != 'Hello Plop!'\n", strp2->str);

  string_append_char(strp2, 'a');
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(strp2->str, "Hello Plop!a"), "FATAL: %s != 'Hello Plop!a'\n", strp2->str);

  FILE *file = fopen("t-vla-string.c","r");
  string_set(strp2, "");
  int ret = string_readline(strp2, file);
  ERROR_FATAL_FMT(-1 == ret, "FATAL: string_readline() = %d'\n", ret);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(strp2->str, "#include <stdlib.h>\n"), "FATAL: %s != '#include <stdlib.h>\\n'\n", strp2->str);
  while ( 0 != feof(file) )
    {
      int ret = string_readline(strp2, file);
      ERROR_FATAL_FMT(-1 == ret, "FATAL: string_readline() = %d'\n", ret);
    }
  
  fclose(file);

  string_set(&str1, "aaaa\n\r\n");
  string_chomp(&str1);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(str1.str, "aaaa"),
			"FATAL: %s != 'aaaa'\n", str1.str);

  string_set(&str1, "aaa");
  string_set(&str2, "aaa");
  ret = string_cmp(&str1, &str2);
  ERROR_UNDEF_FATAL_FMT(0 != ret, "FATAL: string_cmp() returned %d\n", ret);

  string_set(&str2, "baa");
  ret = string_cmp(&str1, &str2);
  ERROR_UNDEF_FATAL_FMT(0 <= ret, "FATAL: string_cmp() returned %d\n", ret);  
  
  return EXIT_SUCCESS;
}
