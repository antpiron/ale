#include "ale/error.h"
#include "ale/parser.h"

int
parser_items_init(struct parser_items *items)
{
  items->n = 0;
  vector_parser_item_init(&items->items);
  
  return 0;
}

void
parser_items_destroy(struct parser_items *items)
{
  for (size_t i = 0 ; i < items->n ; i++)
    if (FOLLOW_UNINITIALIZED != items->items.data[i].followType)
      bitset_destroy(&items->items.data[i].follow);
  
  vector_parser_item_destroy(&items->items);
 }

ssize_t
parser_items_add(struct parser_items *items, size_t rule, size_t dot,
		 unsigned int isCore,
		 unsigned int followType, unsigned int plusFollow, struct bitset *follow)
{
  ssize_t index_ret = items->n++;
  struct parser_item *item = vector_parser_item_get_ptr(&items->items, index_ret);
  
  item->rule = rule;
  item->dot = dot;
  item->isCore = isCore;
  item->followType = followType;
  item->plusFollow = plusFollow;
  
  if (FOLLOW_UNINITIALIZED != followType)
    {
      ERROR_RET( -1 == bitset_init(&item->follow, 64), -1);
      if (NULL != follow)
	{
	  bitset_cpy(&item->follow, follow);
	}
    }

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

/* int */
/* parser_item_set_closure(struct parser_item_set *item_set, struct parser_grammar *g) */
/* { */
/*   struct bitset closure; */

/*   bitset_init(&closure,  g->n_rules); */
  
/*   for (size_t i = 0 ; i < item_set->n ; i++) */
/*     { */
/*       struct parser_item *item = item_set->items.data + i; */
/*       struct grammar_rule *rule = g->rules.data + item->rule; */

/*       if (item->dot < rule->n_rhs) */
/* 	{ */
/* 	  struct grammar_rule_node *node = rule->rhs.data + item->dot; */

/* 	  if (GRAMMAR_NON_TERMINAL == node->type) */
/* 	    { */
/* 	      for (size_t j = 0 ; j < g->n_rules ; j++) */
/* 		{ */
/* 		  if (node->index == g->rules.data[j].lhs) */
/* 		    bitset_set(&closure, j); */
/* 		} */
/* 	    } */
/* 	} */
/*     } */

/*   bitset_destroy(&closure); */
  
/*   return 0; */
/* } */

void
parser_item_set_destroy(struct parser_item_set *item_set)
{
  bitset_destroy(&item_set->elems);
}
