#include <stdlib.h>

#include "error.h"
#include "btree.h"

static struct btnode* 
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

static struct btnode* 
bt_freenode(struct btnode *node)
{
  free(node->key);
  free(node->childs.data);
  free(node);

  return node;
}


int 
bt_init(struct btree *bt, int order, struct btfuncs funcs)
{
  ERROR_CUSTOM_RET(order < 4, -1, BT_ERRNO_INVALID_ORDER);
  bt->order = order;
  ERROR_RET(NULL == (bt->root = bt_mknode(order, BT_NODE_LEAF)), -1);
  bt->f = funcs;

  return ERR_SUCCESS;
}

void
bt_destroy(struct btree *bt)
{
  
}

static int
bt_split_child(struct btree *bt, struct btnode *node, int index)
{
  struct btnode *child = node->childs.nodes[index];
  struct btnode *right_node;
  int m = (bt->order - 1) / 2;
  
  ERROR_RET(NULL == (right_node = bt_mknode(bt->order, child->type)), -1);

  // Init right node
  for (int i = m+1 ; i < bt->order - 1 ; i++)
    {
      right_node->key[i-m] = child->key[i];
      right_node->childs.nodes[i-m] = child->childs.nodes[i];
    }
  right_node->order = bt->order - m - 1;

  // shift parent node
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
  child->order = m + 1;

  return ERR_SUCCESS;
}

static int
bt_split_root(struct btree *bt)
{
  struct btnode *newroot = bt_mknode(bt->order, BT_NODE_INTERNAL);
  ERROR_RET(NULL == newroot, -1);

  newroot->order = 1;
  newroot->childs.nodes[0] = bt->root;

  return ERR_SUCCESS;
}

#define OPT_SPLIT 1
#define OPT_MERGE (1 << 1)

static struct btnode*
bt_search_leaf(struct btree *bt, void *key, int *index, int *found, int options)
{
  struct btnode *node = bt->root;
  
  if ( (options & OPT_SPLIT) && BT_ISFULL(*bt,*bt->root))
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

      if ( (options & OPT_SPLIT) && BT_ISFULL(*bt,*node->childs.nodes[i]))
	ERROR_RET(-1 == bt_split_child(bt, node, i), NULL);
	
      node = node->childs.nodes[i];
    } 

  return node;
}

void* 
bt_search(struct btree *bt, void *key)
{
  int index, found;
  struct btnode *leaf = bt_search_leaf(bt, key, &index, &found, 0);

  return found?leaf->childs.data[index]:NULL;
}


int
bt_insert(struct btree *bt, void *key, void *data)
{
  int index, found;
  struct btnode *leaf;

  leaf = bt_search_leaf(bt, key, &index, &found, OPT_SPLIT);
  
  return ERR_SUCCESS;
}
