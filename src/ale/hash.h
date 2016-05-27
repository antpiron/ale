#ifndef __HASH_H
#define __HASH_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <ale/siphash24.h>

#define HASH_DEFAULT_SIZE ( (1 << 16) + 1 )
#define HASH_MAX_COL ( 1 << 4 )

static inline size_t
hash_func_int(int buf, const uint8_t *key)
{
  uint64_t hashed;

  siphash((uint8_t *) &hashed, (uint8_t*) &buf, (uint64_t) sizeof(buf), key);
  
  return (size_t) hashed;
}

static inline int
equal_func_int(int a, int b)
{
  return a == b;
}
   
// Keyed hash function size_t hash_func(keytype buf, const uint8_t *key)
// int (*equal)(keytype a, keytype b)
#define HASH_INIT(name,keytype,valuetype,equal_func,hash_func)		\
  struct hash_##name##_kv						\
  {									\
    ssize_t index;							\
    keytype key;							\
    valuetype value;							\
  };									\
									\
  struct hash_##name							\
  {									\
    uint8_t keys[2][SIP_KEYLEN];					\
    size_t size;							\
    struct hash_##name##_kv *array;					\
  };									\
									\
  static inline size_t							\
  hash_##name##_hash(struct hash_##name *hash, keytype buf)		\
  {									\
    return hash_func(buf, hash->keys[0]) % hash->size;			\
  }									\
									\
  static inline size_t							\
  hash_##name##_hash_increment(struct hash_##name *hash, keytype buf)	\
  {									\
    size_t ret = hash_func(buf, hash->keys[1]) %			\
      (hash->size / HASH_MAX_COL);					\
    return ret?ret:1;							\
  }									\
									\
  static inline void							\
  hash_##name##_init_size(struct hash_##name *hash, size_t size)	\
  {									\
    hash->size = size;							\
    hash->array = malloc(sizeof(struct hash_##name##_kv) * size);	\
    ERROR_UNDEF_FATAL(NULL == hash->array, "hash_init() Failed to allocate memory\n"); \
    for (size_t i = 0 ; i < size ; i++)					\
      hash->array[i].index = -1;					\
  }									\
									\
  static inline void							\
  hash_##name##_init(struct hash_##name *hash)				\
  {									\
    hash_##name##_init_size(hash, HASH_DEFAULT_SIZE);			\
  }									\
									\
  static inline void							\
  hash_##name##_destroy(struct hash_##name *hash)			\
  {									\
    free(hash->array);							\
  }									\
									\
  static inline void							\
  hash_##name##_destroy_full(struct hash_##name *hash,			\
			     void (*destroy_key_func)(keytype),		\
			     void (*destroy_value_func)(valuetype))	\
  {									\
    for (size_t i = 0 ; i < hash->size ; i++)				\
      {									\
	if (-1 != hash->array[i].index)					\
	  {								\
	    if (NULL != destroy_key_func)				\
	      destroy_key_func(hash->array[i].key);			\
	    if (NULL != destroy_value_func)				\
	      destroy_value_func(hash->array[i].value);			\
	  }								\
      }									\
    free(hash->array);							\
  }									\
									\
  static inline int							\
  hash_##name##_get(struct hash_##name *hash, keytype key, valuetype *value) \
  {									\
    size_t index = hash_##name##_hash(hash, key);			\
    size_t first_index = index;						\
    size_t increment;							\
    size_t i = 0;							\
    do									\
      {									\
	if (-1 == hash->array[index].index)				\
	  return 0;							\
									\
	if ( first_index == hash->array[index].index &&			\
	     equal_func(hash->array[index].key, key) )			\
	  {								\
	    *value = hash->array[index].value;				\
	    return 1;							\
	  }								\
									\
	i++;								\
	if (i >= HASH_MAX_COL)						\
	  return 0;							\
	if ( 1 == i)							\
	  increment = hash_##name##_hash_increment(hash, key);		\
	index += increment;						\
      }									\
    while (1);								\
    /* Never reach here */						\
    ERROR_UNDEF_RET(1, -1);						\
  }									\
									\
  static inline int							\
  hash_##name##_set(struct hash_##name *hash, keytype key,		\
		    valuetype value, valuetype *oldvalue)		\
  {									\
    size_t index = hash_##name##_hash(hash, key);			\
    size_t first_index = index;						\
    size_t increment;							\
    size_t i = 0;							\
    do									\
      {									\
	if (-1 == hash->array[index].index)				\
	  {								\
	    hash->array[index].key = key;				\
	    hash->array[index].value = value;				\
	    hash->array[index].index = first_index;			\
	    return 0;							\
	  }								\
									\
	if ( first_index == hash->array[index].index &&			\
	     equal_func(hash->array[index].key, key) )			\
	  {								\
	    if (NULL != oldvalue)					\
	      *oldvalue = hash->array[index].value;			\
	    hash->array[index].value = value;				\
	    return 1;							\
	  }								\
									\
	i++;								\
	if (i >= HASH_MAX_COL)						\
	  break;							\
	if ( 1 == i)							\
	  increment = hash_##name##_hash_increment(hash, key);		\
	index += increment;						\
      }									\
    while (1);								\
									\
    ERROR_UNDEF_RET(1, -1);						\
  }									\
									\
  static inline int							\
  hash_##name##_delete(struct hash_##name *hash, keytype key,		\
		       keytype *oldkey, valuetype *oldvalue)		\
  {									\
    size_t index = hash_##name##_hash(hash, key);			\
    size_t first_index = hash_##name##_hash_increment(hash, key);	\
    size_t increment = 0;						\
    size_t i = 0;							\
    									\
    for ( i = 0 ; i < HASH_MAX_COL ; i++, index += increment )		\
      {									\
	if (-1 == hash->array[index].index)				\
	  return 0;							\
									\
	if ( first_index == hash->array[index].index &&			\
	     equal_func(hash->array[index].key, key) )			\
	  break;							\
      }									\
    if (i == HASH_MAX_COL)						\
      return 0;								\
									\
    if (NULL != oldkey)							\
      *oldkey = hash->array[index].key;					\
    if (NULL != oldvalue)						\
      *oldvalue = hash->array[index].value;				\
    									\
    /* shift keys */							\
    size_t shiftto = index;						\
    for (i++ ; i < HASH_MAX_COL ; i++)					\
      {									\
	index += increment;						\
	if (-1 == hash->array[index].index)				\
	  break;							\
	if ( first_index != hash->array[index].index )			\
	  continue;							\
									\
	hash->array[shiftto].index = first_index;			\
	hash->array[shiftto].key = hash->array[index].key;		\
	hash->array[shiftto].value = hash->array[index].value;		\
	shiftto = index;						\
      }									\
    hash->array[index].index = -1;					\
    									\
    return 1;								\
  }									\

#endif
