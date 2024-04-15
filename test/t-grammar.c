#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/parser.h"

int
main(int argc, char *argv[argc])
{
  struct parser_grammar g;
  ssize_t rule_S;

  grammar_init(&g);

  rule_S = grammar_add_rule_va(&g,
			       grammar_add_nonterminal(&g, "S"),
			       NULL);
  
  grammar_destroy(&g);
  
  return EXIT_SUCCESS;
}
