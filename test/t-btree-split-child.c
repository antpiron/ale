#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "error.h"
#include "btree.h"

#define ORDER 128
#define RIGHT_ORDER (ORDER / 2)
#define LEFT_ORDER ((ORDER - 1) / 2)

int
main(int argc, char *argv[argc])
{
  struct btree bt;
  struct btnode *child = bt_mknode(ORDER, BT_NODE_LEAF);
  struct btnode *dad = bt_mknode(1, BT_NODE_INTERNAL);
  struct btnode *res;

  ERROR_FATAL(-1 == bt_init(&bt, ORDER, btfuncs_intptr_t), "FAIL: bt_init()");
  ERROR_FATAL(NULL == child || NULL == dad, "FAIL: bt_mknode()");

  for (int i = 0 ; i < ORDER - 1 ; i++)
    {
      child->key[i] =  (void*) (intptr_t) i;
      child->childs.data[i] = (void*) (intptr_t) (i+ORDER);
    }
  dad->order = 1;
  dad->childs.nodes[0] = child;
  
  ERROR_FATAL(-1 == bt_split_child(&bt, dad, 0), "FAIL: bt_split_child");
  ERROR_UNDEF_FATAL_FMT(2 != dad->order, "FAIL: dad order %d != 2", dad->order);
  ERROR_UNDEF_FATAL_FMT(LEFT_ORDER != dad->key[0], "FAIL: dad key %"PRIdPTR" != %d\n", 
			(intptr_t) dad->key[0], LEFT_ORDER);
  ERROR_UNDEF_FATAL_FMT(LEFT_ORDER != dad->childs.nodes[0]->order, "FAIL: left order %d != %d\n", 
			dad->childs.nodes[0]->order, LEFT_ORDER);
  ERROR_UNDEF_FATAL_FMT(RIGHT_ORDER != dad->childs.nodes[1]->order, "FAIL: right order %d != %d\n", 
			dad->childs.nodes[1]->order, RIGHT_ORDER);
   
  bt_freenode(dad->childs.nodes[1]);
  bt_freenode(dad->childs.nodes[0]);
  bt_freenode(dad);
  bt_destroy(&bt);

  return EXIT_SUCCESS;
}
