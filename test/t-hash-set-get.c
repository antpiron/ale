#include <stdlib.h>

#include "ale/error.h"
#include "ale/hash.h"


// generate come collision
#define NUMBER (1 << 18)

void
destroy_kv(struct hash_kv *kv)
{
  // "plop"
  if (4 == kv->keysize)
    return;

  free(kv->key);
}
  
int
main(int argc, char *argv[argc])
{
  struct hash hash;
  struct sl_node *node;
  intptr_t key, value;
  size_t count, keysize;
  struct hash_kv *kv_p;
  struct hash_kv kv;
  int ret;

  // ---------------------------- intptr_t
  ERROR_UNDEF_FATAL(0 != hash_init_full(&hash, HASH_DEFAULT_SIZE, &hash_intptr_t_funcs),
  		    "FAIL: hash_init_full()\n" );
  
  ERROR_UNDEF_FATAL_FMT(0 != (ret = hash_set(&hash, (void*)(intptr_t) 1, 0, (void*)(intptr_t) 1, &kv)),
			"FAIL: hash_set() returned %d\n", ret);

  count = 0;
  for (size_t i = 0 ; i < HASH_DEFAULT_SIZE ; i++)
    {
      if (NULL != hash.array[i].next)
	{
	  node = hash.array[i].next;
	  count++;
	}
    }
  ERROR_UNDEF_FATAL_FMT(1 != count, "FATAL: 1 != %zu", count);

  
  kv_p = (struct hash_kv*) node->data;
  key = (intptr_t) kv_p->key;
  value = (intptr_t) kv_p->value;
  keysize = kv_p->keysize;
  ERROR_UNDEF_FATAL_FMT(1 != key, "FATAL: key %"PRIdPTR" != 1", key);
  ERROR_UNDEF_FATAL_FMT(1 != value, "FATAL: value %"PRIdPTR" != 1", value);
  ERROR_UNDEF_FATAL_FMT(0 != keysize, "FATAL: keysize %zu != 0", keysize);
    
  ERROR_UNDEF_FATAL_FMT(1 != (ret = hash_set(&hash, (void*)(intptr_t) 1, 0, (void*)(intptr_t) 1, &kv)),
			"FAIL: hash_set() returned %d\n", ret);

 
  ERROR_UNDEF_FATAL(-1 == hash_get(&hash, (void*)(intptr_t) 1, 0, &kv),
		    "FATAL: hash_get returned -1\n");
  key = (intptr_t) kv.key;
  value = (intptr_t) kv.value;
  keysize = kv.keysize;
  ERROR_UNDEF_FATAL_FMT(1 != key, "FATAL: key %"PRIdPTR" != 1", key);
  ERROR_UNDEF_FATAL_FMT(1 != value, "FATAL: value %"PRIdPTR" != 1", value);
  ERROR_UNDEF_FATAL_FMT(0 != keysize, "FATAL: keysize %zu != 0", keysize);

  for (intptr_t i = 0 ; i <  NUMBER ; i++)
    {
      ERROR_UNDEF_FATAL(-1 == hash_set(&hash, (void*)(intptr_t) i, 0, (void*)(intptr_t) i, NULL),
  			"FAIL: hash_set() returned -1\n");
    }

  for (intptr_t i = 0 ; i <  NUMBER ; i++)
    {
      ERROR_UNDEF_FATAL(-1 == hash_get(&hash, (void*)(intptr_t) i, 0, &kv),
  			"FAIL: hash_get() returned -1\n");
      key = (intptr_t) kv.key;
      value = (intptr_t) kv.value;
      keysize = (intptr_t) kv.keysize;
      ERROR_UNDEF_FATAL_FMT(i != key, "FATAL: key %"PRIdPTR" != %"PRIdPTR"\n", key, i);
      ERROR_UNDEF_FATAL_FMT(i != value, "FATAL: value %"PRIdPTR" != %"PRIdPTR"\n", value, i);
      ERROR_UNDEF_FATAL_FMT(0 != keysize, "FATAL: keysize %zu != 0", keysize);
    }
  
  hash_destroy(&hash);


  // ---------------------------- Strings
  ERROR_UNDEF_FATAL(0 != hash_init(&hash), "FAIL: hash_init()\n" );
  
  ERROR_UNDEF_FATAL_FMT(0 != (ret = hash_set(&hash, "plop", strlen("plop"), "plop", &kv)),
			"FAIL: hash_set() returned %d\n", ret);
  ERROR_UNDEF_FATAL(-1 == hash_get(&hash, "plop", strlen("plop"), &kv),
		    "FATAL: hash_get returned -1\n");
  char *keystr = (char*) kv.key;
  char *valuestr = (char*) kv.value;
  keysize = kv.keysize;
  ERROR_UNDEF_FATAL_FMT(0 != strcmp("plop", keystr), "FATAL: key %s != plop", keystr);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp("plop", valuestr), "FATAL: value %s != plop", valuestr);
  ERROR_UNDEF_FATAL_FMT(strlen("plop") != keysize, "FATAL: keysize %zu != %zu", strlen("plop"), keysize);

  for (unsigned long i = 0 ; i <  NUMBER ; i++)
    {
      char *str = malloc(32);
      sprintf(str, "plop%08lu", i);
      
      ERROR_UNDEF_FATAL(-1 == hash_set(&hash, (void*) str, strlen(str), (void*) str, NULL),
  			"FAIL: hash_set() returned -1\n");
    }

  for (unsigned long i = 0 ; i <  NUMBER ; i++)
    {
      char str[32];
      sprintf(str, "plop%08lu", i);
      
      ERROR_UNDEF_FATAL(-1 == hash_get(&hash, (void*) str, strlen(str), &kv),
  			"FAIL: hash_get() returned -1\n");
      keystr = (char*) kv.key;
      valuestr = (char*) kv.value;
      keysize = kv.keysize;
      ERROR_UNDEF_FATAL_FMT(0 != strcmp(str, keystr), "FATAL: key %s != %s", keystr, str);
      ERROR_UNDEF_FATAL_FMT(0 != strcmp(str, valuestr), "FATAL: value %s != %s", valuestr, str);
      ERROR_UNDEF_FATAL_FMT(strlen(str) != keysize, "FATAL: keysize %zu != %zu\n", keysize, strlen(str));
    }

  hash_destroy(&hash);
  
  return EXIT_SUCCESS;
}
