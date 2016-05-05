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
