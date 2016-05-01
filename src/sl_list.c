#include <stdlib.h>
#include "error.h"
#include "ale/sl_list.h"

struct sl_node*
sl_mknode(void *data, struct sl_node *next)
{
  struct sl_node *node = malloc(sizeof(struct sl_node));
  ERROR_UNDEF_RET(NULL == node, NULL);

  node->data = data;
  node->next = next;
  
  return node;
}

int
sl_init(struct sl_node *node)
{
  node->next = NULL;
  return 0;
}

int
sl_destroy(struct sl_node *node)
{
  struct sl_node *cur = node->next;
  struct sl_node *next = NULL;
  while (NULL != cur)
    {
      next = cur->next;
      free(cur);
      cur = next;
    }

  node->next = NULL;
  return 0;
}

int
sl_insert(struct sl_node *node, size_t pos, void *data)
{
  struct sl_node *prev = node;
  struct sl_node *new; 

  for (size_t i = 0 ;
       i < pos && NULL != prev->next ;
       i++, prev = prev->next) ;

  new = sl_mknode(data, prev->next);
  ERROR_RET(NULL == new, -1);
  prev->next = new;
  
  return 0;
}

void*
sl_remove(struct sl_node *node, size_t pos)
{
  struct sl_node *prev = node;
  struct sl_node *next;
  void *data;
  size_t i;
  
  for (i = 0 ;
       i < pos && NULL != prev->next ;
       i++, prev = prev->next) ;

  if ( i != pos )
    return NULL;

  data = prev->next->data;
  next = prev->next->next;
  free(prev->next);
  prev->next = next;
  
  return data;
}

struct sl_node*
sl_first(struct sl_node *node)
{
  return node->next;
}

struct sl_node*
sl_next(struct sl_node *node)
{
  return node->next;
}

struct sl_node*
sl_nth(struct sl_node *node, size_t pos)
{
  struct sl_node *cur = node->next;
  
  for (size_t i = 0 ;
       i < pos && NULL != cur ;
       i++, cur = cur->next) ;

  return cur;
}

struct sl_node*
sl_search(struct sl_node *node, void *data, int (*equal)(void*,void*))
{  
  struct sl_node *cur = node->next;
  
  for ( ; NULL != cur && ! equal(data, cur->data) ; cur = cur->next) ;

  return cur;
}

int
sl_push(struct sl_node *node, void *data)
{
  struct sl_node *new = sl_mknode(data, node->next);
  ERROR_RET(NULL == new, -1);
  
  node->next = new;

  return 0;
}

void*
sl_pop(struct sl_node *node)
{
  void* data;
  struct sl_node *top = node->next;
    
  if (NULL == top)
    return NULL;

  node->next = top->next;
  data = top->data;
  free(top);
  
  return data;
}

void*
sl_top(struct sl_node *node)
{
  struct sl_node *top = node->next;
    
  if (NULL == top)
    return NULL;

  return top->data;
}
