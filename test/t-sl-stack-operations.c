#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "ale/error.h"
#include "ale/sl_list.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
  struct sl_node lst;
  intptr_t val;

  ERROR_UNDEF_FATAL(0 != sl_init(&lst), "FAIL: sl_init()" );
  ERROR_UNDEF_FATAL(NULL != lst.next, "FAIL: lst.next != NULL");

  for (intptr_t i = 0 ; i < 100 ; i++)
    {
      ERROR_FATAL(-1 == sl_push(&lst, (void*) i), "FAIL: sl_push()");
      val = (intptr_t) lst.next->data;
      ERROR_FATAL_FMT(val != i, "FAIL: sl_push() val %"PRIdPTR" != %"PRIdPTR"\n", val, i);
    }

  val = (intptr_t) sl_top(&lst);
  ERROR_UNDEF_FATAL(99 != val, "FAIL: sl_top()");
  for (intptr_t i = 99 ; i > -1 ; i--)
    {
      val = (intptr_t) sl_pop(&lst);
      ERROR_FATAL_FMT(i != val, "FAIL: sl_pop() val %"PRIdPTR" != %"PRIdPTR"\n", val, i);
    }

  sl_destroy(&lst);
  ERROR_UNDEF_FATAL(NULL != lst.next, "FAIL: sl_destroy()\n");

  return EXIT_SUCCESS;
}
