%{
#define YY_NO_INPUT
#define YY_NO_UNPUT
#include <stdio.h>
#include "csv_tokens.h"

%}

%option noyywrap
%option prefix="csv"
%option outfile="csv_lexer.c"

%%
[^",\n\r]*                                              { 
                                                          csvlval = csvtext; 
                                                          return CSV_FIELD; 
                                                        } 
\"([^"]|\"\")*\"                                        { 
                                                          csvlval = clean_escaped(csvtext); 
                                                          return CSV_FIELD; 
                                                        }
,                                                       { return CSV_COMMA; }
\r?\n                                                   { return CSV_EOL; }
.                                                       { 
                                                          csvlval = csvtext; 
                                                          return CSV_OTHER; 
                                                        }

%%

