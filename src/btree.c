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
  ERROR_CUSTOM_RET(order < 4, -1, BT_ERRNO_INVALID_ORDER);
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

  bt_slice(right_node, node, from, bt->order-from);

  right_node->order = bt->order - from;

  return right_node;
}

struct btnode *
bt_split_right_internal(struct btree *bt, struct btnode *node, int from)
{
  struct btnode *right_node = bt_mknode(bt->order, node->type);
  int nkeys = bt->order - from - 1;

  ERROR_RET(NULL == right_node, NULL);

  bt_slice(right_node, node, from, nkeys);

  right_node->childs.nodes[nkeys] = node->childs.nodes[bt->order-1];
  right_node->order = bt->order - from;

  return right_node;
}

inline struct btnode *
bt_split_right(struct btree *bt, struct btnode *node, int from)
{
  return BT_ISLEAF(*node)?bt_split_right_leaf(bt, node, from):
                          bt_split_right_internal(bt, node, from);
}


int
bt_split_child(struct btree *bt, struct btnode *node, int index)
{
  struct btnode *child = node->childs.nodes[index];
  struct btnode *right_node;
  int m = (bt->order - 1) / 2;
  
  ERROR_RET(NULL == (right_node = bt_split_right(bt, node, m+1)), -1);

  // shift parent node, always internal node
  for (int i = node->order-1 ; i > index ; i--)
    {
      node->key[i] = node->key[i-1]; 
      node->childs.nodes[i+1] = node->childs.nodes[i]; // Copy right pointer
    }
  node->childs.nodes[index+1] = right_node;

  if ( BT_ISLEAF(*child) )
    ERROR_UNDEF_RET(NULL == (node->key[index] = bt->f.dupkey(child->key[m])), (bt_freenode(right_node), -1));
  else
    node->key[index] = child->key[m];

  node->order++;
  child->order = m;

  return ERR_SUCCESS;
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
