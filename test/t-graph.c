#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "ale/error.h"
#include "ale/graph.h"

#define N (1ull << 8)

GRAPH_TRAVERSAL(test,int,NULL,NULL, void)

int
main(int argc, char *argv[argc])
{
  struct graph g;
  ssize_t n_out;
  ssize_t root;
  ssize_t *nodes = malloc(sizeof(ssize_t) * N) ;

  graph_init(&g);
  ERROR_UNDEF_FATAL_FMT(0 != g.n_nodes, "FAIL: graph_init(), n_nodes == %zu != 0\n",  g.n_nodes);
  ERROR_UNDEF_FATAL_FMT(0 != g.n_edges, "FAIL: graph_init(), n_edges == %zu != 0\n",  g.n_edges);

  root = graph_add_node(&g);
  ERROR_UNDEF_FATAL_FMT(1 != g.n_nodes, "FAIL: graph_add_node(), n_nodes == %zu != 1\n",  g.n_nodes);
  ERROR_UNDEF_FATAL_FMT(0 != root, "FAIL: graph_add_node(), root == %zd != 0\n",  root);

  n_out = graph_n_out(&g, root);
  ERROR_UNDEF_FATAL_FMT(0 != n_out, "FAIL:  graph_n_out(root), n_out == %zd != 0\n",  n_out);

  for (size_t i = 0 ; i < N ; i++)
    {
      nodes[i] = graph_add_node(&g);
      graph_add_edge(&g, root, nodes[i]);
    }
  ERROR_UNDEF_FATAL_FMT(N + 1 != g.n_nodes, "FAIL: graph_add_node(), n_nodes == %zu != %d\n",  g.n_nodes, N + 1);
  ERROR_UNDEF_FATAL_FMT(N != g.n_edges, "FAIL: graph_add_edge(), n_edges == %zu != %d\n",  g.n_edges, N );

  n_out = graph_n_out(&g, root);
  ERROR_UNDEF_FATAL_FMT(N != n_out, "FAIL:  graph_n_out(root), n_out == %zd != %d\n",  n_out, N);

  ssize_t out = -1;
  ssize_t out_i = 1;
  for ( ; graph_iterate_edges(&g, root, &out) ;  out_i++)
    {
      ERROR_UNDEF_FATAL_FMT(out != out_i, "FAIL:  graph_iterate_edges(root), out == %zd != %zd\n",  out, out_i);
    }

  ERROR_UNDEF_FATAL_FMT(N + 1 != out_i, "FAIL:  graph_iterate_edges(root), out == %zd != %d\n",  out, N + 1);
  
  graph_destroy(&g);

  free(nodes);
  
  return EXIT_SUCCESS;
}
