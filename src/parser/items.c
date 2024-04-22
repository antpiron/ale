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

void
parser_item_set_destroy(struct parser_item_set *item_set)
{
  for (size_t i = 0 ; i < item_set->n ; i++)
    parser_item_destroy(item_set->items.data + i);

  vector_parser_item_destroy(&item_set->items);
}
