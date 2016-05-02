#include <string.h>
#include "hash.h"
#include "error.h"

#define DEFAULT_SIZE 65537

size_t
hash_buffer(void *key, size_t keysize)
{
  size_t *sbuf = (size_t*) key;
  char *cbuf = (char*) key;
  size_t res = 0;
  size_t div = keysize / sizeof(size_t);
  size_t mod = keysize % sizeof(size_t);

  for (size_t i = 0 ; i < div ; i++ )
    res ^= sbuf[i];

  for (size_t i = 0 ; i < mod ; i++ )
    res ^= cbuf[i] << (i*8);

  return res % DEFAULT_SIZE;
}

int
equal_buffer(void *a, size_t asize, void *b, size_t bsize)
{
  if (asize != bsize)
    return 0;

  return 0 == memcmp(a, b, asize);  
}

void*
dupkey_buffer(void *key, size_t keysize)
{
  void *nkey = malloc(keysize);

  memcpy(nkey, key, keysize);

  return nkey;
}

void
freekey_buffer(void *key, size_t keysize)
{
  free(key);
}


struct hash_funcs hash_buffer_funcs = { .hash = hash_buffer,
					.equal = equal_buffer,
					.dupkey = dupkey_buffer,
					.freekey = freekey_buffer};

int
hash_init(struct hash *hash)
{
  return hash_init_full(hash, DEFAULT_SIZE, &hash_buffer_funcs);
}

int
hash_init_full(struct hash *hash, size_t size, struct hash_funcs *funcs)
{
  hash->size = size;
  hash->funcs = funcs;

  hash->array = malloc(sizeof(struct sl_node) * size);
  ERROR_UNDEF_RET(NULL == hash->array, -1);

  for (size_t i = 0 ; i < size ; i++)
    sl_init(hash->array+i);
  
  return 0;
}

int
hash_destroy(struct hash *hash)
{
  for (size_t i = 0 ; i < hash->size ; i++)
    sl_destroy(hash->array+i);
  free(hash->array);
}

struct hash_kv {
  void *key;
  size_t keysize;
  void *value;
};

static int
kc_equal(void *a, void *b, void *cls)
{
  struct hash_kv *aa = a;
  struct hash_kv *bb = b;
  int (*equal)(void *a, size_t asize, void *b, size_t bsize) = cls;

  return equal(aa->key, aa->keysize, bb->key, bb->keysize);
}

void*
hash_get(struct hash *hash, void *key, size_t keysize)
{
  size_t hashed_key = hash->funcs->hash(key, keysize);
  struct hash_kv kv_tmp = { .key = key, .keysize = keysize };
  struct sl_node *node = sl_search_full(hash->array + hashed_key, &kv_tmp, kc_equal, hash->funcs->equal);

  if (NULL == node)
    return NULL;

  struct hash_kv *kv = node->data;
  
  return kv->value;
}

void*
hash_set(struct hash *hash, void *key, size_t keysize, void *value)
{
  size_t hashed_key = hash->funcs->hash(key, keysize);
  struct hash_kv kv_tmp = { .key = key, .keysize = keysize };
  struct sl_node *node;
  void *data = NULL;

  node = sl_search_full(hash->array + hashed_key, &kv_tmp, kc_equal, hash->funcs->equal);
  if (NULL != node)
    {
      struct hash_kv *kv = node->data;
      data = kv->value;
      kv->value = value;
    }
  else
    {
      struct hash_kv *kv = malloc(sizeof(struct hash_kv));
      ERROR_UNDEF_FATAL(NULL == kv, "Out of memory in malloc\n");
      kv->key = hash->funcs->dupkey(key, keysize);
      kv->keysize = keysize;
      kv->value = value;
      ERROR_FATAL(-1 == sl_push(hash->array + hashed_key, kv), "sl_push() failed in hash_set()");
    }

  return data;
}
