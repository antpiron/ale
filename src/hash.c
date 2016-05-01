#include "hash.h"
#include "error.h"

#define DEFAULT_SIZE 65537

int
hash_init(struct hash *hash, struct hash_funcs funcs)
{
  return hash_init_full(hash, DEFAULT_SIZE, funcs);
}

int
hash_init_full(struct hash *hash, size_t size, struct hash_funcs funcs)
{
  hash->size = size;
  hash->funcs = funcs;

  hash->array = malloc(sizeof(struct sl_node) * size);
  ERROR_UNDEF_RET(NULL == hash->array, -1);
  
  return 0;
}

int
hash_destroy(struct hash *hash)
{
  free(hash->array);
}

void*
hash_get(struct hash *hash, void *key)
{
}

void*
hash_set(struct sl_node *node, void *key, void *value)
{
}
