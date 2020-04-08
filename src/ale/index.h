#ifndef __INDEX_H
#define __INDEX_H

#include <ale/stringutils.h>
#include <ale/vector.h>
#include <ale/hash.h>


struct index
{
  ssize_t max;
  struct string strings; 
};

void index_init(struct index *index);
void index_destroy(struct index *index);

int index_set(struct index *index, const char *key, size_t i);
ssize_t index_get(struct index *index, const char *key);
char* index_rget(struct index *index, size_t i);


#endif
