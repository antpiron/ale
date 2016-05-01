#ifndef __HASH_H
#define __HASH_H

#include <stdlib.h>
#include <ale/sl_list.h>

struct hash_funcs {
  int (*equal)(void*,void*);
  int (*hash)(void*);
};

struct hash {
  size_t size;
  struct hash_funcs funcs;
  struct sl_list *array;
};

int hash_init(struct hash *hash, int (*equal)(void*,void*));
int hash_init_full(struct hash *hash, size_t size, struct hash_funcs funcs);
int hash_destroy(struct hash *hash);

void* hash_get(struct hash *hash, void *key);
void* hash_set(struct sl_node *node, void *key, void *value);


#endif
