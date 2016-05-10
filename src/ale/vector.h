#ifndef __VECTOR_H
#define __VECTOR_H

#include <stdlib.h>
#include <inttypes.h>

#define VECTOR_DEFAULT_SIZE (256)

#define VECTOR_INIT(name,type)						\
  struct vector_##name							\
  {									\
    size_t size;							\
    type *data;								\
  };									\
  									\
  static inline int							\
  vector_##name##_init(struct vector_##name *vector)			\
  {									\
    vector->data = malloc(sizeof(type) * VECTOR_DEFAULT_SIZE);		\
    ERROR_UNDEF_FATAL(NULL == vector->data,				\
  		      "Unable to allocate memory in vector_init()\n");	\
    									\
    vector->size = VECTOR_DEFAULT_SIZE;					\
    return 0;								\
  }								 	\
  									\
  static inline void									\
  vector_##name##_destroy(struct vector_##name *vector)			\
  {									\
    free(vector->data);							\
  }									\
  									\
  static inline void							\
  vector_##name##_set(struct vector_##name *vector, size_t pos, type value) \
  {									\
    if (vector->size <= pos)						\
      {									\
  	const size_t newsize = ((VECTOR_DEFAULT_SIZE + pos) / VECTOR_DEFAULT_SIZE) * VECTOR_DEFAULT_SIZE; \
  	ERROR_UNDEF_FATAL(NULL == (vector->data = realloc(vector->data, sizeof(type) * newsize)), \
  			  "Unable to allocate memory in vector_init()\n"); \
  	vector->size = newsize;						\
      }									\
    vector->data[pos] = value;						\
  }									\
  									\
  static inline type							\
  vector_##name##_get(struct vector_##name *vector, size_t pos)		\
  {									\
    return vector->data[pos];						\
  }

#endif
