#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ale/stringutils.h"
#include "ale/error.h"

char* 
strjoin(const char *sep, const char **str)
{
  size_t size = 256;
  size_t res_len = 0;
  char* res = malloc(size);
  ERROR_UNDEF_RET(NULL == res, NULL);

  if (*str)
    {
      size_t sep_len = strlen(sep);

      while (1)
	{
	  for (const char *ch = *str ; '\0' != *ch ; ch++, res_len++)
	    {
	      if (res_len >= size-1)
		{
		  size *= 2; 
		  ERROR_UNDEF_RET(NULL == (res = realloc(res, size)), (free(res), NULL));
		}
	      res[res_len] = *ch;
	    }

	  if ( NULL == *++str)
	    break;

	  if (res_len + sep_len >= size-1)
	    {
	      while (res_len + sep_len >= size-1)
		size *= 2; 
	      ERROR_UNDEF_RET(NULL == (res = realloc(res,size)), (free(res), NULL));
	    }

	  memcpy(res+res_len, sep, sep_len);
	  res_len += sep_len;
	}
    }

  res[res_len] = '\0';

  return res;
}

void
strtolower(char *dst, const char *src)
{
  for ( ; *src ; dst++, src++)
    *dst = tolower(*src);
  *dst = '\0';
}

int
string_init(struct string *string)
{
  return string_init_size(string, STRING_DEFAULT_SIZE);
}

int
string_init_size(struct string *string, size_t size)
{
  string->len = 0;

  if ( size > 0)
    {
      string->str = malloc(size);
      ERROR_UNDEF_FATAL(NULL == string->str,
			"string_init() unable to allocate memory!\n");
    }

  string->alloc_size = size;

  return 0;
}

void
string_destroy(struct string *string)
{
  free(string->str);
}

struct string *
string_new(char *str)
{
  struct string *string = malloc(sizeof(struct string));
  ERROR_UNDEF_RET(NULL == string, NULL);

  string_init_size(string, 0);
  string_set(string, str);

  return string;
}

void
string_free(struct string *string)
{
  string_destroy(string);
  free(string);
}


static void
str_resize(struct string *dst, size_t len)
{
  if (len + 1 > dst->alloc_size)
    {
      size_t new_size = ((len + 1 + STRING_DEFAULT_SIZE) / STRING_DEFAULT_SIZE) * STRING_DEFAULT_SIZE;

      if (dst->alloc_size > 0)
	dst->str = realloc(dst->str, new_size);
      else
	dst->str = malloc(new_size);
      ERROR_UNDEF_FATAL(NULL == dst->str, "string_append() unable to allocate memory!\n");
      dst->alloc_size = new_size;      
    }
}

int
string_set(struct string *dst, char *str)
{
  size_t len = strlen(str);

  str_resize(dst, len);

  memcpy(dst->str, str, len);
  dst->str[len] = '\0';
  dst->len = len;
  
  return 0;
}

int
string_append(struct string *dst, struct string *src)
{
  size_t len = dst->len + src->len;

  str_resize(dst, len);

  memcpy(dst->str + dst->len, src->str, src->len);
  dst->str[len] = '\0';
  dst->len = len;
    
  return 0;
}

int
string_append_c(struct string *dst, const char *src)
{
  size_t src_len = strlen(src);
  size_t len = dst->len + src_len;

  str_resize(dst, len);

  memcpy(dst->str + dst->len, src, src_len);
  dst->str[len] = '\0';
  dst->len = len;
    
  return 0;
}

int
string_append_char(struct string *dst, const char src)
{
   size_t len = dst->len + 1;
   
   str_resize(dst, len);
   
   dst->str[dst->len] = src;
   dst->str[len] = '\0';
   dst->len = len;

   return 0;
}

ssize_t
string_readline(struct string *dst, FILE *file)
{
  int c;
  
  string_set(dst, "");
  do
    {
      c = getc(file);
      if ( EOF == c )
	{
	  ERROR_FERROR_RET(0 != ferror(file), -1);
	  break;
	}
	
      string_append_char(dst, c);
    }
  while ('\n' != c);
  
  return dst->len;
}

int
string_chomp(struct string *dst)
{
  ssize_t len = dst->len - 1;

  while ( len >= 0 )
    {
      char c = dst->str[len];

      if ('\n' != c &&  '\r' != c)
	break;

      len--;
    }
  
  dst->str[len+1] = 0;
  dst->len = len+1;
  
  return 0;
}
