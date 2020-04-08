#include <stdlib.h>
#include "ale/memory.h"

void
mem_block_init_size(struct mem_block *block, size_t size)
{
  block->block = malloc(size);
  block->usage = 0;
  block->alloc_size = size;
}

void
mem_block_destroy(struct mem_block *block)
{
  free(block->block);
}

struct mem_block *
mem_block_new(size_t size)
{
  struct mem_block *block = malloc(sizeof(struct mem_block));
  mem_block_init_size(block, size);

  return block;
}

void
mem_block_free(struct mem_block *block)
{
  mem_block_destroy(block);
  free(block);
}

void
mem_init_size(struct mem_pool *pool, size_t size)
{
  pool->block_size = size;
  sl_init(&pool->stack);
}

void
mem_destroy(struct mem_pool *pool)
{
  sl_destroy_full(&pool->stack,
		  (void (*)( void * )) mem_block_free);
}


void *
mem_malloc(struct mem_pool *pool, size_t size)
{
  struct mem_block *block;
  void *ptr;
  
  if ( -1 == sl_top(&pool->stack, (void **) &block) ||
       (block->alloc_size - block->usage) < size )
    {
      
      block = mem_block_new( (size > pool->block_size)?size:pool->block_size );
      sl_push(&pool->stack,  block);
    }

  ptr = block->block + block->usage;
  block->usage += size;
  
  return ptr;
}
