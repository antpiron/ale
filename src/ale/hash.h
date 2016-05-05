#ifndef __HASH_H
#define __HASH_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <ale/sl_list.h>
#include <ale/siphash24.h>

#define HASH_DEFAULT_SIZE 65537

struct hash_funcs {
  size_t (*hash)(const void *key, size_t keysize, const uint8_t *hash_func_key);
  int (*equal)(const void *keya, size_t keyasize, const void *keyb, size_t keybsize);
};

extern struct hash_funcs hash_string_funcs;
extern struct hash_funcs hash_intptr_t_funcs;

struct hash {
  size_t size;
  struct hash_funcs *funcs;
  uint8_t *hash_func_key;
  struct sl_node *array;
};

struct hash_kv {
  void *key;
  size_t keysize;
  void *value;
};

int hash_init(struct hash *hash);
int hash_init_full(struct hash *hash, size_t size, struct hash_funcs *funcs);
void hash_destroy(struct hash *hash);
void hash_destroy_full(struct hash *hash, void (*destroy_kv)(struct hash_kv*));

// Keysize just there for faster hash
int hash_get(struct hash *hash, void *key, size_t keysize, struct hash_kv *kv);
int hash_set(struct hash *hash, void *key, size_t keysize, void *value, struct hash_kv *kv);
int hash_del(struct hash *hash, void *key, size_t keysize, struct hash_kv *kv);

void hash_foreach();

#endif
