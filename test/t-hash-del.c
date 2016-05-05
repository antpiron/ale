#include <stdlib.h>

#include "ale/error.h"
#include "ale/hash.h"


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

  
 
  ERROR_FATAL(-1 == hash_del(&hash, (void*)(intptr_t) 1, 0, &kv),
	      "FATAL: hash_del returned -1\n");
  key = (intptr_t) kv.key;
  value = (intptr_t) kv.value;
  keysize = kv.keysize;
  ERROR_UNDEF_FATAL_FMT(1 != key, "FATAL: key %"PRIdPTR" != 1", key);
  ERROR_UNDEF_FATAL_FMT(1 != value, "FATAL: value %"PRIdPTR" != 1", value);
  ERROR_UNDEF_FATAL_FMT(0 != keysize, "FATAL: keysize %zu != 0", keysize);

  ERROR_FATAL(-1 != hash_get(&hash, (void*)(intptr_t) 1, 0, &kv),
	      "FATAL: hash_get returned 0\n");
  
  hash_destroy(&hash);

  
  return EXIT_SUCCESS;
}
