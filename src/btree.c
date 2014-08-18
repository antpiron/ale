#include <stdlib.h>

#include "error.h"
#include "btree.h"


struct btnode* 
bt_mknode(int btorder, int type)
{
  struct btnode *node = malloc(sizeof(struct btnode));
  ERROR_UNDEF_RET(NULL == node, NULL);
  
  node->type = type;
  node->order = 0;

  ERROR_UNDEF_RET(NULL == (node->key = malloc((btorder-1) * sizeof(void*))), (free(node), NULL));
  ERROR_UNDEF_RET(NULL == (node->childs.data = malloc(btorder * sizeof(void*))), (free(node->key), free(node), NULL));

  return node;
}

void
bt_freenode(struct btnode *node)
{
  free(node->key);
  free(node->childs.data);
  free(node);
}


int 
bt_init(struct btree *bt, int order, struct btfuncs funcs)
{
  ERROR_CUSTOM_RET(order < 4, BT_ERRNO_INVALID_ORDER, -1);
  bt->order = order;
  bt->root = NULL;
  bt->f = funcs;

  return ERR_SUCCESS;
}

void
bt_destroy(struct btree *bt)
{
  
}

void
bt_slice(struct btnode *dest, struct btnode *src,  int from, int count)
{
  // copy left childs and keys
  for (int i = 0, si = from; i < count ; i++, si++)
    {
      dest->key[i] = src->key[si];
      dest->childs.nodes[i] = src->childs.nodes[si];
    }
}

struct btnode *
bt_split_right_leaf(struct btree *bt, struct btnode *node, int from)
{
  struct btnode *right_node = bt_mknode(bt->order, node->type);

  ERROR_RET(NULL == right_node, NULL);

  bt_slice(right_node, node, from, node->order - from);

  right_node->order = node->order - from;
  node->order = from;

  return right_node;
}

struct btnode *
bt_split_right_internal(struct btree *bt, struct btnode *node, int from)
{
  struct btnode *right_node = bt_mknode(bt->order, node->type);
  int nkeys = node->order - from - 1;

  ERROR_RET(NULL == right_node, NULL);

  bt_slice(right_node, node, from, nkeys);

  right_node->childs.nodes[nkeys] = node->childs.nodes[node->order-1];
  right_node->order = node->order - from;
  node->order = from;

  return right_node;
}

inline struct btnode *
bt_split_right(struct btree *bt, struct btnode *node, int from)
{
  return BT_ISLEAF(*node)?bt_split_right_leaf(bt, node, from):
                          bt_split_right_internal(bt, node, from);
}

void
bt_internal_node_insert(struct btnode *node, int index, void *key, struct btnode *right_node)
{
  for (int i = node->order-1 ; i > index ; i--)
    {
      node->key[i] = node->key[i-1]; 
      node->childs.nodes[i+1] = node->childs.nodes[i]; // Copy rights pointers
    }
  node->childs.nodes[index+1] = right_node;
  node->key[index] = key;
  node->order++;
}

int
bt_split_leaf_child(struct btree *bt, struct btnode *node, int index)
{
  struct btnode *child = node->childs.nodes[index];
  struct btnode *right_node;
  int m = child->order / 2;
  void *key;

  ERROR_RET(NULL == (right_node = bt_split_right_leaf(bt, child, m)), -1);
  
  error.type = ERR_SUCCESS;
  key = bt->f.dupkey(child->key[m]);
  ERROR_RET(ERR_SUCCESS != error.type, (bt_freenode(right_node), -1));

  bt_internal_node_insert(node, index, key, right_node);

  return ERR_SUCCESS;
}

int
bt_split_internal_child(struct btree *bt, struct btnode *node, int index)
{
  struct btnode *child = node->childs.nodes[index];
  struct btnode *right_node;
  int m = child->order / 2;

  ERROR_RET(NULL == (right_node = bt_split_right_internal(bt, child, m)), -1);

  bt_internal_node_insert(node, index, child->key[m], right_node);

  return ERR_SUCCESS;
}

int
bt_split_child(struct btree *bt, struct btnode *node, int index)
{
  struct btnode *child = node->childs.nodes[index];

  return BT_ISLEAF(*child)?bt_split_leaf_child(bt, node, index):
                           bt_split_internal_child(bt, node, index); 
}

int
bt_split_root(struct btree *bt)
{
  struct btnode *newroot = bt_mknode(bt->order, BT_NODE_INTERNAL);
  ERROR_RET(NULL == newroot, -1);

  newroot->order = 1;
  newroot->childs.nodes[0] = bt->root;

  return ERR_SUCCESS;
}


struct btnode*
bt_search_leaf(struct btree *bt, void *key, int *index, int *found, int options)
{
  struct btnode *node = bt->root;
  
  if ( (options & BT_OPT_SPLIT) && BT_ISFULL(*bt,*bt->root))
    ERROR_RET(-1 == bt_split_root(bt), NULL);

  while (1)
    {
      int i = 0;
      int cmp = -1;

      while ( i < node->order - 1 && 
	      0 < (cmp = bt->f.cmpkey(node->key[i], key)) )
	i++;

      if (BT_ISLEAF(*node))
	{
	  *found = (0 == cmp);
	  *index = i;
	  break;
	}

      if ( (options & BT_OPT_SPLIT) && BT_ISFULL(*bt,*node->childs.nodes[i]))
	ERROR_RET(-1 == bt_split_child(bt, node, i), NULL);
	
      node = node->childs.nodes[i];
    } 

  return node;
}

void* 
bt_search(struct btree *bt, void *key)
{
  int index, found;
  struct btnode *leaf;

  if (NULL == bt->root)
    return NULL;

  leaf = bt_search_leaf(bt, key, &index, &found, 0);

  return found?leaf->childs.data[index]:NULL;
}


int
bt_insert(struct btree *bt, void *key, void *data)
{
  int index, found;
  struct btnode *leaf;

  if (NULL == bt->root)
    ERROR_RET(NULL == (bt->root = bt_mknode(bt->order, BT_NODE_LEAF)), -1);

  ERROR_RET(NULL == (leaf = bt_search_leaf(bt, key, &index, &found, BT_OPT_SPLIT)), -1);
 
  if ( ! found )
    for (int i = leaf->order-1 ; i > index ; i--)
      {
	leaf->key[i] = leaf->key[i-1]; 
	leaf->childs.data[i] = leaf->childs.data[i-1];
      }

  leaf->key[index] = key;
  leaf->childs.data[index] = data;
 
  return ERR_SUCCESS;
}
