#include "ale/symbols.h"


void
ale_symbols_init(struct ale_symbols *s)
{
  mem_init(&s->pool);
  
  sl_init(&s->tables);
  mem_callback(&s->pool, &s->tables, (void (*)(void *)) sl_destroy);
  
  s->top = NULL;
}


void
ale_symbols_destroy(struct ale_symbols *s)
{  
  mem_destroy(&s->pool);
}

struct ale_symbol_table*
ale_symbols_new_table(struct ale_symbols *s, const char *name, struct ale_symbol_table *parent)
{
  struct ale_symbol_table *st = mem_malloc(&s->pool, sizeof(struct ale_symbol_table));

  if ( NULL != name )
    {
      size_t len = strlen(name);
      st->name = mem_malloc(&s->pool, len + 1);
      strcpy(st->name, name);
    }
  else
    st->name = NULL;
  
  st->parent = parent;
  st->n = 0;
  
  index_init(&st->index);
  mem_callback(&s->pool, &st->index, (void (*)(void *)) index_destroy);
  
  vector_ale_symbol_init(&st->symbols);
  mem_callback(&s->pool, &st->symbols, (void (*)(void *)) vector_ale_symbol_destroy);
  
  return st;
}

struct ale_symbol_table*
ale_symbols_push_table(struct ale_symbols *s, const char *name)
{
  struct ale_symbol_table *st = ale_symbols_new_table(s, name, s->top);

  sl_push(&s->tables, st);
  s->top = st;
  
  return st;
}
