#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "ale/error.h"
#include "ale/graph.h"


int
main(int argc, char *argv[argc])
{
  struct graph g;
  ssize_t root, n1;

  graph_init(&g);
  ERROR_UNDEF_FATAL_FMT(0 != g.n_nodes, "FAIL: graph_init(), n_nodes == %zu != 0\n",  g.n_nodes);
  ERROR_UNDEF_FATAL_FMT(0 != g.n_edges, "FAIL: graph_init(), n_edges == %zu != 0\n",  g.n_edges);

  root = graph_add_node(&g);
  ERROR_UNDEF_FATAL_FMT(1 != g.n_nodes, "FAIL: graph_add_node(), n_nodes == %zu != 1\n",  g.n_nodes);
  ERROR_UNDEF_FATAL_FMT(0 != root, "FAIL: graph_add_node(), root == %zd != 0\n",  root);
  
  graph_destroy(&g);
  
  return EXIT_SUCCESS;
}
