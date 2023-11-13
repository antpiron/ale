#include "ale/symbols.h"


void
ale_symbols_init(struct ale_symbols *s)
{
  mem_init(&s->pool);
  
  s->parent = s->parent.next = NULL;
}


void
ale_symbols_destroy(struct ale_symbols *s)
{
  mem_destroy(&s->pool);
}
