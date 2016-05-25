#ifndef __ALE_STRINGUTILS_H
#define __ALE_STRINGUTILS_H

#include <string.h>

char* strjoin(const char *sep, const char **str); 
void strtolower(char *dst, const char *src); 

#define STRING_DEFAULT_SIZE (256)
struct string
{
  char *str;
  size_t len;
  size_t alloc_size;
};

int string_init(struct string *string);
int string_init_size(struct string *string, size_t size);
void string_destroy(struct string *string);

struct string *string_new(char *str);
void string_free(struct string *string);

int string_set(struct string *dst, char *str);
int string_append(struct string *dst, struct string *src);
int string_append_c(struct string *dst, const char *src);

#endif
