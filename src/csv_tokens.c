#include <string.h>
#include <inttypes.h>

#include "csv_tokens.h"

char *csvlval;

char *
clean_escaped(char *str)
{
  int prev = 0;
  size_t shift = 1;
  char *cur = str + 1;
 
  for ( ; *cur ; cur++)
    {
      *(cur-shift) = *cur;
 
      if ('"' == *cur)
	{
	  if(prev)
	    {
	      shift++;
	      prev = 0;
	    }
	  else
	    prev = 1;
	}
    }
  
  *(cur - shift - 1) = '\0';

  return str;
}
