#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/parser.h"
#include "ale/stringutils.h"

/*
  https://lambda.uta.edu/cse5317/notes/node18.html
  0) S ::= R $
  1) R ::= R b
  2) R ::= a
  
  state          action 	        	goto
    	|        a 	b 	$ 	| 	S 	R
  0 	| 	 s3 	  	  	|	  	1
  1 	| 	  	s4 	s2	| 	  	 
  2 	| 	 a 	a 	a	| 	  	 
  3  	|	 r2 	r2 	r2	|	  	 
  4 	| 	 r1 	r1 	r1	|
*/

ssize_t
goto_table(size_t state, size_t lhs, void *cls)
{
  
}


enum { RULE_S_R = 0, RULE_R_Rb, RULE_R_a };

struct parser_action*
action_table(size_t state, size_t token, void *cls)
{
  static struct parser_action pa = { .type = PARSER_ACTION_ERROR  };
  
  if ( 0 == state && 'a' == token )
    {
      pa.type = PARSER_ACTION_SHIFT;
      pa.shift.next_state = 3;
    }

  if ( 1 == state )
    {
      if ( 'b' == token )
	{
	  pa.type = PARSER_ACTION_SHIFT;
	  pa.shift.next_state = 4;
	}
      if ( '\0' == token )
	{
	  pa.type = PARSER_ACTION_SHIFT;
	  pa.shift.next_state = 2;
	}
    }

  if ( 2 == state )
    pa.type = PARSER_ACTION_ACCEPT;

  if ('a' == token || 'b' == token || '\0' == token)
    {
      
      if ( 3 == state )
	{
	  pa.type = PARSER_ACTION_REDUCE;
	  pa.reduce.lhs = RULE_R_a;
	  pa.reduce.rhs_n = 1;
	  pa.reduce.callback = NULL;
	}
      
      if ( 4 == state )
	{
	  pa.type = PARSER_ACTION_REDUCE;
	  pa.reduce.lhs = RULE_R_a;
	  pa.reduce.rhs_n = 2;
	  pa.reduce.callback = NULL;
	}
    }
  
  return &pa;
}

struct input
{
  size_t pos;
  char *str;
};

struct lexer_token*
next_token(size_t state, void *cls)
{
  static char *str = "a";
  static struct lexer_token tok;
  struct input *in = cls;

  tok.str = str;
  
  switch (in->str[in->pos])
    {
      
    case '\0':
      tok.id = 0;
      tok.str[0] = 0; 
      break;

    default:
      tok.id = tok.str[0] =in->str[in->pos++];
      break;      
    }
}

int
main(int argc, char *argv[argc])
{

  
  return EXIT_SUCCESS;
}
