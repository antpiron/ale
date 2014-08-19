#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "error.h"
#include "btree.h"

#define ORDER 17
#define LEAF_ORDER (ORDER - 1)
#define RIGHT_ORDER ((LEAF_ORDER+1) / 2 + 1)
#define LEFT_ORDER (LEAF_ORDER / 2)

int
main(int argc, char *argv[argc])
{
  struct btree bt;

  ERROR_FATAL(-1 == bt_init(&bt, ORDER, btfuncs_intptr_t), "FAIL: bt_init()");

  ERROR_FATAL(-1 ==  bt_insert(&bt, (void*) (intptr_t) 0, (void*)(intptr_t) 0), "FAIL: unable to insert (0 => 0)");
  ERROR_UNDEF_FATAL_FMT(1 != bt.root->order, "FAIL: Order %d != 1", bt.root->order);

  for (int i = 1 ; i < LEAF_ORDER ; i++)
    ERROR_FATAL_FMT(-1 ==  bt_insert(&bt, (void*) (intptr_t) i, (void*)(intptr_t) i), 
		    "FAIL: unable to insert (%d => %d)", i, i);

  ERROR_UNDEF_FATAL_FMT(LEAF_ORDER != bt.root->order, "FAIL: Order %d != %d", bt.root->order, LEAF_ORDER);

  ERROR_FATAL_FMT(-1 ==  bt_insert(&bt, (void*) (intptr_t) LEAF_ORDER, (void*)(intptr_t) LEAF_ORDER), 
		  "FAIL: unable to insert (%d => %d)", LEAF_ORDER, LEAF_ORDER);

  ERROR_UNDEF_FATAL_FMT(2 != bt.root->order, "FAIL: Order %d != 2", bt.root->order);
  ERROR_UNDEF_FATAL_FMT(LEFT_ORDER != bt.root->childs.nodes[0]->order, "FAIL: Left order %d != %d", 
			bt.root->childs.nodes[0]->order, LEFT_ORDER);
  ERROR_UNDEF_FATAL_FMT(RIGHT_ORDER != bt.root->childs.nodes[1]->order, "FAIL: Right order %d != %d", 
			bt.root->childs.nodes[1]->order, RIGHT_ORDER);

  bt_destroy(&bt);

  return EXIT_SUCCESS;
}
