#ifndef __MEMORY_H
#define __MEMORY_H

#include <ale/sl_list.h>
#include <stdint.h>


#define MEM_DEFAULT_BLOCK_SIZE (1l << 16)

struct mem_block
{
  size_t alloc_size;
  size_t usage;
  char *block;
};

struct mem_pool
{
  size_t block_size;
  struct sl_node stack;
};


void mem_block_init_size(struct mem_block *block, size_t size);
void mem_block_destroy(struct mem_block *block);

struct mem_block *mem_block_new(size_t size);
void mem_block_free(struct mem_block *block);

void mem_init(struct mem_pool *pool);
void mem_init_size(struct mem_pool *pool, size_t size);
void mem_destroy(struct mem_pool *pool);

void *mem_malloc(struct mem_pool *pool, size_t size);


#endif
