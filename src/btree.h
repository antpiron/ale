#ifndef __BTREE_H
#define __BTREE_H

#include <stdint.h>

typedef int (*bt_cmp_func_t)(void*, void*);
typedef void (*bt_freekey_func_t)(void*);
typedef void* (*bt_dupkey_func_t)(void*);

#define BT_NODE_LEAF (1)
#define BT_NODE_INTERNAL (0)
#define BT_ISLEAF(node) ((node).type & BT_NODE_LEAF)
#define BT_ISINTERNAL(node) (! BT_ISLEAF(node))

#define BT_ISFULL(bt,node) ( (BT_ISLEAF(node) && (node).order == (bt).order -1) || (node).order == (bt).order)

enum {
  BT_ERRNO_INVALID_ORDER = 1
};

struct btnode {
  int type;      // internal, leaf
  int order;
  void **key;  
  union {
    void **data;
    struct btnode **nodes;
  } childs;
};

struct btfuncs {
  bt_cmp_func_t cmpkey;
  bt_dupkey_func_t dupkey;
  bt_freekey_func_t freekey;
};

struct btree 
{
  int order;
  struct btnode *root;
  struct btfuncs f;
};


int bt_init(struct btree *bt, int order, struct btfuncs funcs);
void bt_destroy(struct btree *bt);


int bt_insert(struct btree *bt, void *key, void *data);
void* bt_search(struct btree *bt, void *key);


#endif
