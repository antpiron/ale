#ifndef __SKIPLIST_H
#define __SKIPLIST_H

#include <stdlib.h>
#include <inttypes.h>

#define SKL_INIT(name,maxlevel,keytype,valuetype,cmp_func)		\
  static inline size_t							\
  skl_randomlevel()							\
  {									\
    uint64_t rnd = rand();						\
    size_t lvl = 0;							\
    									\
    while ( 1ull == (rnd & 1) && lvl < maxlevel )			\
      { lvl++; rnd >>= 1; }						\
    									\
    return lvl+1;							\
  }									\
									\
  struct skl_##name##_node						\
  {									\
    keytype key;							\
    valuetype value;							\
    struct skl_##name##_node *forward[maxlevel];			\
  };									\
  									\
  static inline struct skl_##name##_node*				\
  skl_##name##_makeNode(size_t newLevel, keytype key, valuetype value)	\
  {									\
    struct skl_##name##_node *node = malloc(sizeof(struct skl_##name##_node)); \
    ERROR_UNDEF_FATAL(NULL == node, "skl_makeNode() Failed to allocate memory\n");	\
									\
    node->key = key;							\
    node->value = value;						\
    									\
    return node;							\
  }									\
									\
  struct skl_##name							\
  {									\
    ssize_t level;							\
    struct skl_##name##_node header;					\
  };									\
									\
  static inline void							\
  skl_##name##_init(struct skl_##name *skl)				\
  {									\
    skl->level = 0;							\
    for (ssize_t i = 0 ; i < maxlevel ; i++)				\
      skl->header.forward[i] = NULL;					\
  }								 	\
  									\
  static inline int							\
  skl_##name##_search(struct skl_##name *skl, keytype key,		\
		      valuetype *value)					\
  {									\
    struct skl_##name##_node *x = &skl->header;				\
    int cmp = -2;							\
									\
    for (ssize_t i = skl->level ; i > -1 ; i--)				\
      {									\
	while (NULL != x->forward[i]					\
	       && -1 == (cmp = cmp_func(key,x->forward[i]->key)) )	\
	  x = x->forward[i];						\
      }									\
									\
    if (0 == cmp)							\
      {									\
	*value = x->forward[0]->value;					\
	return 1;							\
      }									\
    else if (-2 == cmp)							\
      return -1;							\
									\
    *value = x->value;								\
    return 0;								\
  }									\
									\
  static inline int							\
  skl_##name##_search_update(struct skl_##name *skl, keytype key,	\
			     struct skl_##name##_node **node,		\
			     struct skl_##name##_node *update[maxlevel]) \
  {									\
    struct skl_##name##_node *x = &skl->header;				\
    int cmp = -1;							\
									\
    for (ssize_t i = skl->level ; i > -1 ; i--)				\
      {									\
	while (NULL != x->forward[i]					\
	       && -1 == (cmp = cmp_func(key,x->forward[i]->key)) )	\
	  x = x->forward[i];						\
	update[i] = x;							\
      }									\
									\
    if (0 == cmp)							\
      {									\
	*node = x->forward[0];						\
	return 1;							\
      }									\
									\
    *node = x;					\
    return 0;								\
  }									\
									\
  static inline int							\
  skl_##name##_insert(struct skl_##name *skl, keytype key,		\
		      valuetype value)					\
  {									\
    struct skl_##name##_node *x;					\
    struct skl_##name##_node *update[maxlevel];				\
									\
    if ( skl_##name##_search_update(skl, key, &x, update) )		\
      {									\
	x->value = value;						\
      }									\
    else								\
      {									\
	size_t newLevel = skl_randomlevel();				\
	if (newLevel > skl->level)					\
	  {								\
	    for (ssize_t i = newLevel-1 ; i >=  skl->level ; i--)	\
	      update[i] = &skl->header;					\
	    skl->level = newLevel;					\
	  }								\
	x = skl_##name##_makeNode(newLevel, key, value);		\
	for (ssize_t i = 0 ; i < newLevel ; i++)			\
	  {								\
	    x->forward[i] = update[i]->forward[i];			\
	    update[i]->forward[i] = x;					\
	  }								\
      }									\
      									\
  }

#endif
