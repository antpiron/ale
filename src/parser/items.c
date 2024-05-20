#include "ale/error.h"
#include "ale/parser.h"

int
parser_items_init(struct parser_items *items)
{
  stack_parser_item_init(&items->items);
  graph_init(&items->genby);
  
  return 0;
}

void
parser_items_destroy(struct parser_items *items)
{
  while ( ! stack_parser_item_isempty(&items->items) )
    {
      struct parser_item *item = stack_parser_item_pop_ptr(&items->items);
      if (FOLLOW_UNINITIALIZED != item->followType)
	bitset_destroy(&item->follow);
    }
  
  stack_parser_item_destroy(&items->items);
  graph_destroy(&items->genby);  
 }

ssize_t
parser_items_add(struct parser_items *items, size_t rule, size_t dot,
		 unsigned int isCore,
		 unsigned int followType, unsigned int plusFollow, struct bitset *follow)
{
  struct parser_item item = { .rule = rule, .dot = dot, .isCore = isCore, .followType = followType,  .plusFollow = plusFollow };

  if (FOLLOW_UNINITIALIZED != followType)
    {
      if (NULL != follow)
	{
	  ERROR_RET( -1 == bitset_init(&item.follow, follow->n), -1);
	  bitset_cpy(&item.follow, follow);
	}
      else
	ERROR_RET( -1 == bitset_init(&item.follow, 0), -1);
    }

  ssize_t index_ret = stack_parser_item_n(&items->items);
  stack_parser_item_push(&items->items, item);

  return index_ret;
}




void
parser_item_set_init(struct parser_item_set *item_set, struct parser_items *items)
{
  item_set->n = 0;
  item_set->items = items;
  bitset_init(&item_set->elems, 0);
}

int
parser_item_set_add(struct parser_item_set *item_set, size_t item)
{
  bitset_set(&item_set->elems, item);
  
  return 0;
}

int
parser_item_set_closure(struct parser_item_set *item_set, struct parser_grammar *g)
{
  struct bitset closure;
  bitset_init(&closure,  g->n_rules);

  ssize_t  i = -1;
  while ( bitset_iterate(&item_set->elems, &i) )
    {
      struct parser_item *item = stack_parser_item_get_ptr(&item_set->items->items, i);
      struct grammar_rule *rule = g->rules.data + item->rule;

      if (item->dot < rule->n_rhs)
	{
	  struct grammar_rule_node *node = rule->rhs.data + item->dot;

	  if (GRAMMAR_NON_TERMINAL == node->type)
	    {
	      for (size_t j = 0 ; j < g->n_rules ; j++)
		{
		  if (node->index == g->rules.data[j].lhs)
		    {
		      /* Cannot return -1 when FOLLOW_UNINITIALIZED */
		      ssize_t new_item_index = parser_items_add(item_set->items, j, 0, 0, FOLLOW_UNINITIALIZED, 0, NULL);

		      /* TODO: update item_set->items->genby to reflect dependencies
			 add edge from new_item_index to i
		       */
		      bitset_set(&closure, new_item_index);
		    }
		}
	    }
	}
    }

  bitset_destroy(&closure);
  
  return 0;
}

void
parser_item_set_destroy(struct parser_item_set *item_set)
{
  bitset_destroy(&item_set->elems);
}
