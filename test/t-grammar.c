#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/parser.h"

int
main(int argc, char *argv[argc])
{
  struct parser_grammar g;
  ssize_t rule_S, rule_T, S, E;

  grammar_init(&g);

  ERROR_UNDEF_FATAL_FMT(-1 != g.start_nt, "grammar_init(): start_nt == %zd != -1\n", g.start_nt);
  ERROR_UNDEF_FATAL_FMT(0 != g.n_terminals, "grammar_init(): n_terminals == %zd != 0\n", g.n_terminals);
  ERROR_UNDEF_FATAL_FMT(0 != g.n_nonterminals, "grammar_init(): n_nonterminals == %zd != 0\n", g.n_nonterminals);
  ERROR_UNDEF_FATAL_FMT(0 != g.n_rules, "grammar_init(): n_rules == %zd != 0\n", g.n_rules);

  S = grammar_add_nonterminal(&g, "S");
  E = grammar_add_nonterminal(&g, "E");
  rule_S = grammar_add_rule_va(&g,
			       S,
			       G_NT(&g, "E"), G_T(&g, "+"),  G_NT(&g, "E"),
			       NULL);
  rule_T = grammar_add_rule_va(&g,
			       E,
			       G_NT(&g, "ID"),
			       NULL);
  grammar_add_rule_va(&g,
		      E,
		      G_NT(&g, "NUMBER"),
		      NULL);

  grammar_print(&g);
  grammar_destroy(&g);
  
  return EXIT_SUCCESS;
}
