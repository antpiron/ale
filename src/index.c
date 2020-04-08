#include "ale/index.h"


void
index_init(struct index *index)
{
  mem_init(&index->pool);
  vector_chars_init(&index->itos);
  hash_chars_size_t_init(&index->stoi);
  index->max = -1;
}

void
index_destroy(struct index *index)
{
  mem_destroy(&index->pool);
  vector_chars_destroy(&index->itos);
  hash_chars_size_t_init(&index->stoi);
}

int
index_set(struct index *index, const struct string *key, size_t i)
{
  char *str = mem_malloc(&index->pool, key->len + 1);
  strcpy(str, key->str); 

  vector_chars_set(&index->itos, i, str);
  int ret = hash_chars_size_t_set(&index->stoi, str, i, NULL);

  if ((ssize_t) i > index->max)
    index->max = i;
  
  return ret;
}

ssize_t
index_get(struct index *index, const char *key)
{
  size_t i;
  int ret = hash_chars_size_t_get(&index->stoi, (char*) key, &i);

  if (0 == ret)
    return -1;
  
  return i;
}

char*
index_rget(struct index *index, size_t i)
{
  return vector_chars_get(&index->itos, i);
}
