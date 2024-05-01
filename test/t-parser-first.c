#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/parser.h"

int
main(int argc, char *argv[argc])
{
  struct parser_grammar g;
  struct parser_first pf;
  ssize_t S, E, T, F, ID, NUMBER;

  grammar_init(&g);

  ERROR_UNDEF_FATAL_FMT(-1 != g.start_nt, "grammar_init(): start_nt == %zd != -1\n", g.start_nt);
  ERROR_UNDEF_FATAL_FMT(1 != g.n_terminals, "grammar_init(): n_terminals == %zd != 1\n", g.n_terminals);
  ERROR_UNDEF_FATAL_FMT(0 != g.n_nonterminals, "grammar_init(): n_nonterminals == %zd != 0\n", g.n_nonterminals);
  ERROR_UNDEF_FATAL_FMT(0 != g.n_rules, "grammar_init(): n_rules == %zd != 0\n", g.n_rules);

  S = grammar_add_nonterminal(&g, "S");
  E = grammar_add_nonterminal(&g, "E");
  T = grammar_add_nonterminal(&g, "T");
  F = grammar_add_nonterminal(&g, "F");
  ID = grammar_add_nonterminal(&g, "ID");
  NUMBER = grammar_add_nonterminal(&g, "NUMBER");
  
  grammar_add_rule_va(&g,
		      S,
		      G_NT(&g, "E"),
		      NULL);

  grammar_add_rule_va(&g,
		      E,
		      G_NT(&g, "E"), G_T(&g, "+"),  G_NT(&g, "T"),
		      NULL);
  grammar_add_rule_va(&g,
		      E,
		      G_NT(&g, "E"), G_T(&g, "-"),  G_NT(&g, "T"),
		      NULL);
   grammar_add_rule_va(&g,
		      E,
		      G_NT(&g, "T"),
		      NULL);
 
  grammar_add_rule_va(&g,
		      T,
		      G_NT(&g, "T"), G_T(&g, "*"),  G_NT(&g, "F"),
		      NULL);
  grammar_add_rule_va(&g,
		      T,
		      G_NT(&g, "T"), G_T(&g, "/"),  G_NT(&g, "F"),
		      NULL);

  grammar_add_rule_va(&g,
		      T,
		      G_NT(&g, "F"),
		      NULL);
  
  grammar_add_rule_va(&g,
		      F,
		      G_NT(&g, "ID"),
		      NULL);
  grammar_add_rule_va(&g,
		      F,
		      G_NT(&g, "NUMBER"),
		      NULL);
  grammar_add_rule_va(&g,
		      F,
		      G_T(&g, "-"), G_NT(&g, "F"),
		      NULL);
  grammar_add_rule_va(&g,
		      F,
		      G_T(&g, "("), G_NT(&g, "E"),  G_T(&g, ")"),
		      NULL);

  grammar_add_rule_va(&g,
		      ID,
		      G_T(&g, "/[a-zA-Z][a-zA-Z0-9_]*/"),
		      NULL);
  grammar_add_rule_va(&g,
		      NUMBER,
		      G_T(&g, "/[0-9]+(\\.[0-9]+)?/"),
		      NULL);

  // grammar_print(&g);

  parser_first_init(&pf, &g);

  parser_first_print(&pf);
  
  parser_first_destroy(&pf);
  
  grammar_destroy(&g);
  
  return EXIT_SUCCESS;
}
