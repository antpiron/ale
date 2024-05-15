#ifndef __GRAPH_H
#define __GRAPH_H

#include <ale/bitset.h>
#include <ale/vector.h>
#include <inttypes.h>

VECTOR_INIT(edges, struct bitset)

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


#endif
