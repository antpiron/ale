%{
#define YY_NO_INPUT
#define YY_NO_UNPUT
#include <stdio.h>
#include "csv_tokens.h"

%}

%option noyywrap
%option prefix="csv"
%option outfile="csv_lexer.c" header-file="csv_lexer.h"

%%
[^",\n\r]*                                              { 
                                                          csvlval = yytext; 
                                                          return CSV_FIELD; 
                                                        } 
\"([^"]|\"\")*\"                                        { 
                                                          csvlval = clean_escaped(yytext); 
                                                          return CSV_FIELD; 
                                                        }
,                                                       { return CSV_COMMA; }
\r?\n                                                   { return CSV_EOL; }
.                                                       { 
                                                          csvlval = yytext; 
                                                          return CSV_OTHER; 
                                                        }

%%

