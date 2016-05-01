#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "error.h"
#include "sl_list.h"

#define ORDER 16

int
equal_intptr_t(void *a, void *b)
{
  return (intptr_t) a == (intptr_t) b; 
}

int
main(int argc, char *argv[argc])
{
  struct sl_node lst;
  struct sl_node *node;
  intptr_t val;

  ERROR_UNDEF_FATAL(0 != sl_init(&lst), "FAIL: sl_init()" );
  ERROR_UNDEF_FATAL(NULL != lst.next, "FAIL: lst.next != NULL");

  for (intptr_t i = 0 ; i < 100 ; i++)
    ERROR_FATAL(-1 == sl_insert(&lst, i, (void*) i), "FAIL: sl_insert()");

  val = 25;
  node = sl_search(&lst, (void*) val, equal_intptr_t);
  ERROR_UNDEF_FATAL(NULL == node, "FAIL: 25 not found!\n");
  ERROR_UNDEF_FATAL_FMT((intptr_t) node->data != val, "FAIL: %"PRIdPTR" != 25\n", val);

  sl_destroy(&lst);
  ERROR_UNDEF_FATAL(NULL != lst.next, "FAIL: sl_destroy()\n");
  
  return EXIT_SUCCESS;
}
