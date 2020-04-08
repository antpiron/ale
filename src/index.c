#include "ale/index.h"


void
index_init(struct index *index)
{
  string_init(&index->strings);
  index->max = -1;
}

void
index_destroy(struct index *index)
{
  string_destroy(&index->strings);
}

int
index_set(struct index *index, const char *key, size_t i)
{
 

  return 0;
}
