#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "ale/error.h"
#include "ale/parser.h"
#include "ale/stringutils.h"


void
grammar_init(struct parser_grammar *g)
{
   mem_init(&g->pool);

   g->n_terminals = g->n_nonterminals = g->n_rules = 0;
   g->start_nt = -1;
   index_init(&g->terminals);
   /* Add epsilon at index 0 */
   grammar_add_terminal(g, "");
   index_init(&g->nonterminals);
   vector_grammar_rule_init(&g->rules);
}

void
grammar_set_start(struct parser_grammar *g, size_t start_nt)
{
  g->start_nt = start_nt;
}


void
grammar_destroy(struct parser_grammar *g)
{
  mem_destroy(&g->pool);
  index_destroy(&g->terminals);
  index_destroy(&g->nonterminals);
  vector_grammar_rule_destroy(&g->rules);
}

void
grammar_print(struct parser_grammar *g)
{
  for (size_t i = 0 ; i < g->n_rules ; i++)
    {
      struct grammar_rule *rule = g->rules.data + i;
      printf("%3zu. %s ::=", i, index_rget(&g->nonterminals, rule->lhs));
      for (size_t j = 0 ; j < rule->n_rhs ; j++)
	{
	  struct grammar_rule_node *node = rule->rhs.data + j;
	  if (GRAMMAR_NON_TERMINAL == node->type)
	    printf(" %s", index_rget(&g->nonterminals, node->index));
	  else if (GRAMMAR_TERMINAL == node->type)
	    printf(" '%s'", index_rget(&g->terminals, node->index));
	}
      printf("\n");
    }
}


inline
static
ssize_t
index_lookup_or_add(struct index *index, const char *str, size_t *n)
{
  ssize_t pos = index_get(index, str);

  if (pos < 0)
    { 
      int ret = index_set(index, str, *n);
      ERROR_RET(ret < 0, -1);

      pos = (*n)++;
    }

  return pos;
}

ssize_t
grammar_add_terminal(struct parser_grammar *g, const char *str)
{
  return index_lookup_or_add(&g->terminals, str, &g->n_terminals);
}

ssize_t
grammar_add_nonterminal(struct parser_grammar *g, const char *name)
{
  return index_lookup_or_add(&g->nonterminals, name, &g->n_nonterminals);
}


ssize_t
grammar_add_rule_va(struct parser_grammar *g, size_t lhs, ...)
{
  va_list ap;
  struct grammar_rule rule;
  
  rule.lhs = lhs;
  rule.n_rhs = 0;

  vector_grammar_rule_node_init(&rule.rhs);
  
  va_start(ap, lhs);
  
  for (struct grammar_rule_node *node = va_arg(ap, struct grammar_rule_node *) ;
       NULL != node ;
       node = va_arg(ap, struct grammar_rule_node *) )
    {
      vector_grammar_rule_node_set(&rule.rhs, rule.n_rhs, *node);
      rule.n_rhs++;
    }
  
  va_end(ap);

  vector_grammar_rule_set(&g->rules, g->n_rules, rule);

  return g->n_rules++;
}

