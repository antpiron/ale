#ifndef __SYMBOLS_H
#define __SYMBOLS_H

#include <stdlib.h>
#include <stddef.h>
#include <ale/memory.h>
#include <ale/stringutils.h>
#include <ale/index.h>
#include <ale/vector.h>

struct ale_symbol
{
  struct mem_pool pool;
  int type;
  char *str;
  void *data;
};

VECTOR_INIT(ale_symbol, struct ale_symbol)


struct ale_symbols
{
  struct index index;
  size_t n;
  struct vector_ale_symbol symbols;
  struct symbols *parent;
};

void ale_symbols_init(struct ale_symbols *s);
struct ale_symbols* ale_symbols_new();
void ale_symbols_destroy(struct ale_symbols *s);
void ale_symbols_free(struct ale_symbols *s);

void ale_symbol_add(struct ale_symbols *s, int type, void *data);

#endif
