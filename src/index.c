#include "ale/index.h"


void
index_init(struct index *index)
{
  mem_init(&index->pool);
  index->max = -1;
}

void
index_destroy(struct index *index)
{
  mem_destroy(&index->pool);
}

int
index_set(struct index *index, const char *key, size_t i)
{
 

  return 0;
}
