#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "ale/portability.h"
#include <string.h>
#include <linux/random.h>
#include "ale/hash.h"
#include "ale/error.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
size_t
hash_intptr_t(const void *key, size_t keysize, const uint8_t *hash_func_key)
{
  uint64_t hashed;
  siphash((uint8_t *) &hashed, (uint8_t*) &key, (uint64_t) sizeof(intptr_t), hash_func_key);
  
  return (size_t) hashed % HASH_DEFAULT_SIZE;
}

int
equal_intptr_t(const void *keya, size_t keyasize, const void *keyb, size_t keybsize)
{
  return (intptr_t) keya == (intptr_t) keyb;  
}
#pragma GCC diagnostic pop

struct hash_funcs hash_intptr_t_funcs = { .hash = hash_intptr_t,
					  .equal = equal_intptr_t };
  
size_t
hash_buffer(const void *key, size_t keysize, const uint8_t *hash_func_key)
{
  uint64_t hashed;
  siphash((uint8_t *) &hashed, (uint8_t*) key, (uint64_t) keysize, hash_func_key);

  return (size_t) hashed % HASH_DEFAULT_SIZE;
}

#define hash_string hash_buffer

int
equal_string(const void *keya, size_t keyasize, const void *keyb, size_t keybsize)
{
  return keyasize == keybsize && 0 == strcmp((char*) keya, (char*) keyb);  
}



struct hash_funcs hash_string_funcs = { .hash = hash_string,
					.equal = equal_string};

int
hash_init(struct hash *hash)
{
  return hash_init_full(hash, HASH_DEFAULT_SIZE, &hash_string_funcs);
}

int
hash_init_full(struct hash *hash, size_t size, struct hash_funcs *funcs)
{
  int eintr_count = 0;
  
  hash->size = size;
  hash->funcs = funcs;
  hash->hash_func_key = malloc(SIP_KEYLEN);
  while ( -1 == getrandom(hash->hash_func_key, SIP_KEYLEN, 0) )
    {
      if (EAGAIN == errno || EINTR == errno)
	{
	  eintr_count++;
	  ERROR_ERRNO_FATAL_FMT(3 <= eintr_count,
				"getrandom() in hash_init_full() returned %d times EINTR\n", eintr_count);
	  continue;
	}

      ERROR_ERRNO_FATAL(1, "getrandom() in hash_init_full() failed\n");
    }

  hash->array = malloc(sizeof(struct sl_node) * size);
  ERROR_UNDEF_RET(NULL == hash->array, -1);

  for (size_t i = 0 ; i < size ; i++)
    sl_init(hash->array+i);
  
  return 0;
}

void
hash_destroy(struct hash *hash)
{
  hash_destroy_full(hash, NULL);
}

void
hash_destroy_full(struct hash *hash, void (*destroy_kv)(struct hash_kv*))
{
  free(hash->hash_func_key);
  for (size_t i = 0 ; i < hash->size ; i++)
    sl_destroy_full(hash->array+i, (void (*)(void *)) destroy_kv);
  free(hash->array);
}

static int
kc_equal(void *a, void *b, void *cls)
{
  struct hash_kv *aa = a;
  struct hash_kv *bb = b;
  int (*equal)(void *a, size_t asize, void *b, size_t bsize) = cls;

  return equal(aa->key, aa->keysize, bb->key, bb->keysize);
}

int
hash_get(struct hash *hash, void *key, size_t keysize, struct hash_kv *kv)
{
  size_t hashed_key = hash->funcs->hash(key, keysize, hash->hash_func_key);
  struct hash_kv kv_tmp = { .key = key, .keysize = keysize };
  struct sl_node *node = sl_search_full(hash->array + hashed_key, &kv_tmp, kc_equal, hash->funcs->equal);
 
  if (NULL == node)
    ERROR_UNDEF_RET(1, -1);

  *kv = *(struct hash_kv *) node->data;
  
  return 0;
}

int
hash_set(struct hash *hash, void *key, size_t keysize, void *value, struct hash_kv *kv)
{
  size_t hashed_key = hash->funcs->hash(key, keysize, hash->hash_func_key);
  struct hash_kv kv_tmp = { .key = key, .keysize = keysize };
  struct sl_node *node;
  struct hash_kv *newkv;
  int ret = 0;

  node = sl_search_full(hash->array + hashed_key, &kv_tmp, kc_equal, hash->funcs->equal);
  if (NULL != node)
    {
      newkv = (struct hash_kv*) node->data;
      ret = 1;
      if (NULL != kv)
	*kv = *newkv;
    }
  else
    {
      ret = 0;
      newkv = malloc(sizeof(struct hash_kv));
      ERROR_UNDEF_FATAL(NULL == newkv, "Out of memory in malloc\n");
      ERROR_FATAL(-1 == sl_push(hash->array + hashed_key, newkv), "sl_push() failed in hash_set()");
    }

  newkv->key = key;
  newkv->keysize = keysize;
  newkv->value = value;

  return ret;
}

int
hash_del(struct hash *hash, void *key, size_t keysize, struct hash_kv *kv)
{
  size_t hashed_key = hash->funcs->hash(key, keysize, hash->hash_func_key);
  struct sl_node *prev = hash->array + hashed_key;

  for ( ; NULL != prev->next ; prev = sl_next(prev))
    {
      struct sl_node *cur = prev->next;
      struct hash_kv *cur_kv = cur->data;
      if ( hash->funcs->equal(cur_kv->key, cur_kv->keysize, key, keysize) )
	{
	  void *tmp;
	  if (NULL != kv)
	    *kv = *cur_kv;
	  ERROR_UNDEF_RET(-1 == sl_pop(prev, &tmp), -1);

	  return 0;
	}
    }

  ERROR_UNDEF_RET(1, -1);
}

void
hash_foreach(struct hash *hash, void (*callback)(struct hash_kv *kv, void *cls), void *cls)
{
  for (size_t i = 0 ; i < hash->size ; i++)
    {
      for (struct sl_node *node = sl_first(hash->array+i) ; NULL != node ; node = sl_next(node))
	callback((struct hash_kv*) node->data, cls);
    }
}
