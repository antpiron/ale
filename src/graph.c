#include "ale/error.h"
#include "ale/graph.h"


void
graph_init(struct graph *g)
{
  g->n_nodes = g->n_edges = 0;
  vector_edges_init(&g->edges);
}

void
graph_destroy(struct graph *g)
{
  for (size_t i = 0 ; i < g->n_nodes ; i++)
    bitset_destroy(g->edges.data + i);
  
  vector_edges_destroy(&g->edges);
}

ssize_t
graph_add_node(struct graph *g)
{
  struct bitset *bs = vector_edges_get_ptr(&g->edges, g->n_nodes);

  ERROR_RET(bitset_init(bs, g->n_nodes + 1) < 0, -1);
  
  return g->n_nodes++;
}

ssize_t
graph_add_edge(struct graph *g, size_t node1, size_t node2)
{
  ERROR_CUSTOM_RET(node1 >= g->n_nodes || node1 >= g->n_nodes, GRAPH_ERROR_OUT_OF_RANGE, -1);
  
  bitset_set(g->edges.data + node1, node2);
  
  return g->n_edges++;
}

int
graph_iterate_edges(struct graph *g, size_t node, ssize_t *state)
{
  if (node >= g->n_nodes)
    return 0;
  
  struct bitset *bs = vector_edges_get_ptr(&g->edges, node);
  
  return bitset_iterate(bs, state);
}

ssize_t
graph_n_out(struct graph *g, size_t node)
{
  ERROR_CUSTOM_RET(node >= g->n_nodes, GRAPH_ERROR_OUT_OF_RANGE, -1);

  struct bitset *bs = vector_edges_get_ptr(&g->edges, node);
  
  return bitset_ones(bs);
}
