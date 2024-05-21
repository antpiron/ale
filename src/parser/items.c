#include "ale/error.h"
#include "ale/parser.h"

int
parser_items_init(struct parser_items *items)
{
  stack_parser_item_init(&items->items);
  graph_init(&items->generated_from);
  
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
  graph_destroy(&items->generated_from);  
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
  item_set->items = items;
  bitset_init(&item_set->elems, 0);
}

int
parser_item_set_add(struct parser_item_set *item_set, size_t item)
{
  bitset_set(&item_set->elems, item);
  
  return 0;
}

ssize_t
parser_item_set_get(struct parser_item_set *item_set, size_t rule, size_t dot)
{
  for (ssize_t  item_i = -1 ; bitset_iterate(&item_set->elems, &item_i) ;  )
    {
      struct parser_item *item = stack_parser_item_get_ptr(&item_set->items->items, item_i);

      if (item->rule == rule && item->dot == dot)
	return item_i;
    }

  return -1;
}

int
parser_item_set_closure(struct parser_item_set *item_set, struct parser_grammar *g)
{
  struct bitset items_todo;

  bitset_init(&items_todo,  item_set->elems.n);
  bitset_cpy(&items_todo, &item_set->elems);
 
  while ( bitset_ones(&items_todo) )
    {
      for (ssize_t  item_i = -1 ; bitset_iterate(&items_todo, &item_i) ;  )
	{
	  struct parser_item *item = stack_parser_item_get_ptr(&item_set->items->items, item_i);
	  struct grammar_rule *rule = g->rules.data + item->rule;

	  bitset_set(&item_set->elems, item_i);
	  bitset_unset(&items_todo, item_i);
	  
	  if (item->dot < rule->n_rhs)
	    {
	      struct grammar_rule_node *node = rule->rhs.data + item->dot;
	      
	      if (GRAMMAR_NON_TERMINAL == node->type)
		{
		  struct bitset *rules = vector_nt_get_ptr(&g->nonterminals_bitsets, node->index);
		  
		  for (ssize_t rule_i = -1 ; bitset_iterate(rules, &rule_i) ; )
		    {
		      ssize_t child_item_i = parser_item_set_get(item_set, rule_i, 0);

		      if ( child_item_i < 0)
			{
			  child_item_i = parser_items_add(item_set->items, rule_i, 0, 0, FOLLOW_UNINITIALIZED, 0, NULL);
			  
			  bitset_set(&items_todo, child_item_i);
			  
			}
		      /* TODO: graph_add_edge(&new_item->generated_from, new_item_i, item_i); */
		    }
		}
	    }
	}
    }

  bitset_destroy(&items_todo);
  
  return 0;
}

void
parser_item_set_destroy(struct parser_item_set *item_set)
{
  bitset_destroy(&item_set->elems);
}
