#include <stdlib.h>

#include "error.h"
#include "btree.h"


int 
bt_intptr_t_cmp(void *a, void *b)
{
  return (intptr_t) b -  (intptr_t) a;
}

void bt_intptr_t_free(void *ptr) {}

void* 
bt_intptr_t_dup(void *a)
{
  return a;
}

struct btfuncs btfuncs_intptr_t = { .cmpkey = bt_intptr_t_cmp, 
				    .dupkey = bt_intptr_t_dup, 
				    .freekey = bt_intptr_t_free };
