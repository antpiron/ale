#include <stdlib.h>

#include <string.h>

#include "ale/error.h"
#include "ale/memory.h"



int
main(int argc, char *argv[argc])
{
  struct mem_pool pool;

  mem_init(&pool);
  ERROR_UNDEF_FATAL(MEM_DEFAULT_BLOCK_SIZE != pool.block_size, "FAIL: mem_init()" );

  for (int i = 0 ; i < 1024 ; i++)
    {
      char *ptr = NULL;
      ptr = mem_malloc(&pool, 1000);
      memset(ptr, i % 256, 1000);
      ERROR_UNDEF_FATAL(NULL == pool.stack.next, "FAIL: mem_malloc()" );
    }
  
  mem_destroy(&pool);
  
  return EXIT_SUCCESS;
}
