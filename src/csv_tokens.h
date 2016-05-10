#ifndef __CSV_TOKENS_H
#define __CSV_TOKENS_H

#include <stdio.h>

enum { CSV_EOF = 0, CSV_COMMA, CSV_FIELD, CSV_EOL, CSV_OTHER };

/*int csvlex();
  extern FILE *csvin;*/
extern char* csvlval;

char *clean_escaped(char *str);

#endif
