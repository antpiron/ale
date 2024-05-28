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
		      
		      ERROR_RET(graph_get_node(&item_set->items->generated_from, item_i, 1) < 0 ||
				graph_get_node(&item_set->items->generated_from, child_item_i, 1) < 0,
				-1);
		      graph_add_edge(&item_set->items->generated_from, child_item_i, item_i);
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


void
parser_graph_init(struct parser_graph *graph, struct parser_grammar *g)
{
  graph->g = g;
  graph->isSlrFollowComputed = 0;
  stack_parser_item_init(&graph->items);
  stack_parser_item_set_init(&graph->sets);
}

void
parser_graph_destroy(struct parser_graph *graph)
{
  struct parser_item *item;
  struct parser_item_set *piset;
  
  while ( NULL != (item = stack_parser_item_pop_ptr(&graph->items)) )
    {
      bitset_destroy(&item->generatedFrom);
      if (FOLLOW_UNINITIALIZED != item->followType)
	bitset_destroy(&item->follow);
    }
  
  while ( NULL != (piset = stack_parser_item_set_pop_ptr(&graph->sets)) )
    {
      bitset_destroy(&piset->elems);
    }

  
  stack_parser_item_destroy(&graph->items);
  stack_parser_item_set_destroy(&graph->sets);

  if (graph->isSlrFollowComputed)
    parser_follow_destroy(&graph->slr_follow);
}

ssize_t
parser_graph_addSet(struct parser_graph *graph)
{
  struct parser_item_set piset = {.isClosed = 0};
  ssize_t index_ret = stack_parser_item_set_n(&graph->sets);

  bitset_init(&piset.elems, 0);
  stack_parser_item_set_push(&graph->sets, piset);
  
  return index_ret;
}

ssize_t
parser_graph_addItem(struct parser_graph *graph, size_t set,
		     size_t rule, size_t dot,  unsigned int isCore)
{
  struct parser_item item = { .rule = rule, .dot = dot, .isCore = isCore, .followType = FOLLOW_UNINITIALIZED,  .plusFollow = 0 };

  bitset_set(&item.generatedFrom, 0);

  ssize_t index_ret = stack_parser_item_n(&graph->items);
  stack_parser_item_push(&graph->items, item);

  struct parser_item_set *piset = stack_parser_item_set_get_ptr(&graph->sets, set);
  bitset_set(&piset->elems, index_ret);

  return index_ret;

}

ssize_t
parser_graph_getItem(struct parser_graph *graph, size_t set,
		     size_t rule, size_t dot)
{
   struct parser_item_set *piset = stack_parser_item_set_get_ptr(&graph->sets, set);
   ssize_t index_ret = -1;

   while (bitset_iterate(&piset->elems, &index_ret))
     {
       struct parser_item *item = stack_parser_item_get_ptr(&graph->items, index_ret);
       
       if (item->rule == rule && item->dot == dot)
	 return index_ret;
     }

   ERROR_CUSTOM_RET(1, PARSER_ERROR_NOT_FOUND, -1);
   return -1;
}

ssize_t
parser_graph_getItemOrSet(struct parser_graph *graph, size_t set,
			  size_t rule, size_t dot)
{
  ssize_t  index_ret = parser_graph_getItem(graph, set, rule, dot);

  if (index_ret < 0)
    {
      index_ret = parser_graph_addItem(graph, set, rule, dot, 0);
    }

  return index_ret;
}

int
parser_graph_setItemFollow(struct parser_graph *graph, size_t item,
			   unsigned int followType, unsigned int plusFollow, struct bitset *follow)
{
  struct parser_item *item_ptr = stack_parser_item_get_ptr(&graph->items, item);

  if (FOLLOW_UNINITIALIZED == item_ptr->followType)
    bitset_init(&item_ptr->follow, follow->n);

  bitset_cpy(&item_ptr->follow, follow);
  item_ptr->followType = followType;
  item_ptr->plusFollow = plusFollow;
  
  
  return 0;
}

struct bitset*
parser_graph_getItemFollow_ptr(struct parser_graph *graph, size_t item)
{
  struct parser_item *item_ptr = stack_parser_item_get_ptr(&graph->items, item);

  return &item_ptr->follow;
}
