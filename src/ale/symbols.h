#ifndef __SYMBOLS_H
#define __SYMBOLS_H

#include <stdlib.h>
#include <stddef.h>
#include <ale/memory.h>
#include <ale/stringutils.h>
#include <ale/index.h>
#include <ale/vector.h>
#include <ale/sl_list.h>

struct ale_symbol
{
  int type;
  char *str;
  void *data;
};

VECTOR_INIT(ale_symbol, struct ale_symbol);

struct ale_symbol_table
{
  char *name;
  struct index index;
  size_t n;
  struct vector_ale_symbol symbols;
  struct ale_symbol_table *parent;
};


struct ale_symbols
{
  struct mem_pool pool;
  struct ale_symbol_table *top;
  struct sl_node tables;
};

void symbols_init(struct ale_symbols *s);
void symbols_destroy(struct ale_symbols *s);

struct ale_symbol_table* ale_symbols_new_table(struct ale_symbols *s, const char *name, struct ale_symbol_table *parent);
struct ale_symbol_table* ale_symbols_push_table(struct ale_symbols *s, const char *name);
struct ale_symbol_table* ale_symbols_pop_table(struct ale_symbols *s);
struct ale_symbol_table* ale_symbols_top_table(struct ale_symbols *s);

struct ale_symbol* ale_symbols_add(struct ale_symbols *s, const char *str, int type, void *ptr);
struct ale_symbol* ale_symbols_get(struct ale_symbols *s, const char *str);


#endif
