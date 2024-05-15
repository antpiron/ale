#ifndef __GRAPH_H
#define __GRAPH_H

#include <ale/bitset.h>
#include <ale/vector.h>
#include <inttypes.h>

VECTOR_INIT(edges, struct bitset)

#ifndef GRAPH_MAX_RECURSION_DEPTH
#  define GRAPH_MAX_RECURSION_DEPTH (1ll << 10)
#endif
  
#define GRAPH_ERROR_OUT_OF_RANGE (1)

struct graph
{
  size_t n_nodes, n_edges;
  struct vector_edges edges;
};

void graph_init(struct graph *g);
void graph_destroy(struct graph *g);

ssize_t graph_add_node(struct graph *g);
ssize_t graph_add_edge(struct graph *g, size_t node1, size_t node2);
int graph_iterate_edges(struct graph *g, size_t node, ssize_t *state);
ssize_t graph_n_out(struct graph *g, size_t node);

#define GRAPH_TRAVERSAL(name,type,pre_func,post_func,func_param_type)	\
  struct graph_traversal_##name						\
  {									\
    struct graph *g;							\
    struct bitset marks;						\
    func_param_type *cls;						\
  };									\
									\
  static TYPE								\
  graph_traversal_##name##_dfs_rec(struct graph_traversal_##name *gt, size_t node, int max_rec_depth) \
  {									\
    TYPE ret;								\
									\
    bitset_set(&gt->marks, node);					\
    pre_func(gt, node);							\
									\
    if (max_rec_depth > 0)						\
      {									\
	ssize_t out = -1;						\
									\
	while ( graph_iterate_edges(&g, node, &out) )			\
	  {								\
	    if ( ! bitset_isset(&gt->marks, out) )			\
	      {								\
		graph_traversal_##name##_dfs_rec(gt, out,		\
						 max_rec_depth - 1);	\
	      }								\
	  }								\
      }									\
    									\
    return post_func(gt, node);						\
  }									\
  									\
  static TYPE								\
  graph_traversal_##name##_dfs(struct graph *g, size_t root, func_param_type *cls) \
  {									\
    struct graph_traversal_##name gt;					\
    TYPE ret;								\
									\
    gt->g = g;								\
    gt->cls = cls;							\
    bitset_init(&gt->marks, g->n_nodes);				\
									\
    ret = graph_traversal_##name##_dfs_rec(g, root, GRAPH_MAX_RECURSION_DEPTH);	\
									\
    bitset_destroy(&gt->marks);						\
									\
    return ret;								\
  }

#endif
