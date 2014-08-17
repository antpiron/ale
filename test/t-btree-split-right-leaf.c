#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "error.h"
#include "btree.h"

#define ORDER 128
#define SPLIT_POS (ORDER / 3)
#define RIGHT_ORDER (ORDER - SPLIT_POS)

int
main(int argc, char *argv[argc])
{
  struct btree bt;
  struct btnode *node = bt_mknode(ORDER, BT_NODE_LEAF);
  struct btnode *res;

  ERROR_FATAL(-1 == bt_init(&bt, ORDER, btfuncs_intptr_t), "FAIL: bt_init()");
  ERROR_FATAL(NULL == node, "FAIL: bt_mknode()");

  for (int i = 0 ; i < ORDER - 1 ; i++)
    {
      node->key[i] =  (void*) (intptr_t) i;
      node->childs.data[i] = (void*) (intptr_t) (i+ORDER);
    }
  
    
  ERROR_FATAL(NULL == (res = bt_split_right_leaf(&bt, node, SPLIT_POS)), 
	      "FAIL: bt_split_right_leaf() returned NULL");

  ERROR_FATAL_FMT(RIGHT_ORDER != res->order, "FAIL: order %d != %d", res->order, RIGHT_ORDER);

  for (int i = 0 ; i < res->order ; i++)
    {
      ERROR_FATAL_FMT(res->key[i] != node->key[SPLIT_POS+i], 
		      "FAIL: keys %"PRIdPTR" != %"PRIdPTR, 
		      (intptr_t) res->key[i], (intptr_t) node->key[SPLIT_POS+i]);
      ERROR_FATAL_FMT(res->childs.data[i] != node->childs.data[SPLIT_POS+i], 
		      "FAIL: data %"PRIdPTR" != %"PRIdPTR, 
		      (intptr_t) res->childs.data[i], (intptr_t) node->childs.data[SPLIT_POS+i]);
    }

  bt_freenode(node);
  bt_freenode(res);
  bt_destroy(&bt);

  return EXIT_SUCCESS;
}
