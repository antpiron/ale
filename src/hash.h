#ifndef __HASH_H
#define __HASH_H

#include <stdlib.h>
#include <ale/sl_list.h>

struct hash_funcs {
  size_t (*hash)(void *key, size_t keysize);
  int (*equal)(void *a, size_t asize, void *b, size_t bsize);
  void* (*dupkey)(void *key, size_t keysize);
  void (*freekey)(void *key, size_t keysize);
};

extern struct hash_funcs hash_buffer_funcs;

struct hash {
  size_t size;
  struct hash_funcs *funcs;
  struct sl_node *array;
};

int hash_init(struct hash *hash);
int hash_init_full(struct hash *hash, size_t size, struct hash_funcs *funcs);
int hash_destroy(struct hash *hash);

void* hash_get(struct hash *hash, void *key, size_t keysize);
void* hash_set(struct hash *hash, void *key, size_t keysize, void *value);

void hash_foreach();

#endif
