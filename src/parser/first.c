#include "ale/error.h"
#include "ale/parser.h"

   /*
    A -> 'a' B C
    A ->
    B ->
    B -> 'b'
    C -> 'c'
    first(A) = { 'c', 'b' }

    first({}) = {EPS}
    first(terminal) = {terminal}
    first(NT) = {first(lhs) |  for all NT1 -> lhs}
    first(NT A B ...) = first(NT)                                  if EPS not in first(NT)
                      = (first{NT} - EPS) union first(A B ...)     if EPS in first(NT)
   */
int
parser_first_init(struct parser_first *pf, struct parser_grammar *g)
{
  struct bitset *first = malloc(sizeof(struct bitset) * g->n_nonterminals);

  pf->g = g;
  pf->first = first;

  for (size_t i = 0 ; i < g->n_nonterminals ; i++)
    bitset_init(first + i, g->n_terminals);
    
  for (size_t r = 0 ; r < g->n_rules ; r++)
    {
      struct grammar_rule *rule = g->rules.data + r;
      struct bitset *rule_first = first + rule->lhs;
      
      if (0 == rule->n_rhs)
	{
	  /* empty rule, add epsilon */
	  bitset_set(rule_first, 0);
	}
      else
	{
	  size_t dot = 0;
	  
	  for ( dot = 0 ; dot < rule->n_rhs ; dot++)
	    {
	      struct grammar_rule_node *node = rule->rhs.data + dot;
	      
	      if (GRAMMAR_TERMINAL == node->type)
		{
		  bitset_set(rule_first, node->index);
		  break;
		}
	      
	       if (GRAMMAR_NON_TERMINAL == node->type)
		 {
		   struct bitset *node_first = first + node->index;
		   
		   bitset_or(rule_first, rule_first, node_first);
		   
		   if (! bitset_isset(node_first, 0))
		     break;
		 }
	    }
	  
	  if (dot == rule->n_rhs)
	    bitset_set(first + rule->lhs, 0);
	}
      
    }

  return 0;
}

void
parser_first_destroy(struct parser_first *pf)
{
  for (size_t i = 0 ; i < pf->g->n_nonterminals ; i++)
    bitset_destroy(pf->first + i);
}

/* int */
/* parser_first(struct parser_grammar *g, */
/* 	     size_t n, struct grammar_rule_node nodes[n], */
/* 	     struct bitset *first) */
/* { */
/*   int ret = 0; */
/*   struct bitset done; */
  
/*   bitset_init(&done, g->n_nonterminals); */
  
/*   // ret = parser_first_rec(g, n, nodes, first, &done); */
  
/*   bitset_destroy(&done); */
  
/*   return ret; */
/* } */
