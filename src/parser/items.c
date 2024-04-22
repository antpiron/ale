#include "ale/error.h"
#include "ale/parser.h"

int
parser_item_init(struct parser_item *item, size_t rule, size_t dot, struct bitset *follow)
{
  item->rule = rule;
  item->dot = dot;
  
  ERROR_RET( -1 == bitset_init(&item->follow, 64), -1);
  if (NULL != follow)
    {
      bitset_cpy(&item->follow, follow);
    }

  return 0;
}

int
parser_item_first(struct parser_item *item, struct parser_grammar *g, struct bitset *first)
{
  struct grammar_rule *rule = g->rules.data + item->rule;

  if (item->dot < rule->n_rhs)
    {
      // TODO: recursively call first on all non-terminal rules
    }
  else if (item->dot == rule->n_rhs)
    {
      bitset_cpy(first, &item->follow);
    }	

  return 0;
}

void
parser_item_destroy(struct parser_item *item)
{
  bitset_destroy(&item->follow);
}


void
parser_item_set_init(struct parser_item_set *item_set)
{
  item_set->n = 0;
  vector_parser_item_init(&item_set->items);
}

int
parser_item_set_add(struct parser_item_set *item_set, size_t rule, size_t dot, struct bitset *follow)
{
  struct parser_item item;

  ERROR_RET( -1 == parser_item_init(&item, rule, dot, follow), -1);
    
  vector_parser_item_set(&item_set->items, item_set->n, item);
  item_set->n++;

  return(0);
}

int
parser_item_set_closure(struct parser_item_set *item_set, struct parser_grammar *g)
{
  for (size_t i = 0 ; i < item_set->n ; i++)
    {
      struct parser_item *item = item_set->items.data + i;
      struct grammar_rule *rule = g->rules.data + item->rule;

      if (item->dot < rule->n_rhs)
	{
	  struct grammar_rule_node *node = rule->rhs.data + item->dot;

	  if (GRAMMAR_NON_TERMINAL == node->type)
	    {
	      // TODO: call parser_item_first() on item->dot + 1
	    }
	}
    }
  
  return 0;
}

void
parser_item_set_destroy(struct parser_item_set *item_set)
{
  for (size_t i = 0 ; i < item_set->n ; i++)
    parser_item_destroy(item_set->items.data + i);

  vector_parser_item_destroy(&item_set->items);
}
