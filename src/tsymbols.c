#include <string.h>

#include "error.h"
#include "tsymbols.h"

#define INITIAL_ALLOC 16

int 
ts_init(struct tsymbols *ts, struct tsymbols *dad)
{
  ts->dad = dad;
  ts->num = 0;
  
  ERROR_ERRNO_RET(NULL == (ts->symbol = malloc(INITIAL_ALLOC * sizeof(*ts->symbol))), -1);
  ts->num_allocated = INITIAL_ALLOC;
   
    
  return ERR_SUCCESS;
}


void 
ts_destroy(struct tsymbols *ts)
{
  free(ts->symbol);
}

 
static void*
bisect(struct tsymbols *ts, char *key)
{
  int s = 0, e = ts->num - 1;
  int m, cmp = -1;

  while (s <= e)
    {
      m = (s+e) / 2;
      cmp = strcmp(key, ts->symbol[m].name);

      if (0 == cmp)
	break;

      if (cmp < 0)
	e = m-1;
      else
	s = m+1;
    }

  return (0 == cmp)?ts->symbol[m].data:NULL;
}

void* 
ts_search(struct tsymbols *ts, char *key)
{
  void *ret = NULL;

  for (struct tsymbols *cur = ts ; NULL == ret && NULL != cur ; cur = cur->dad)
    ret = bisect(cur, key);
  
  return ret;
}

int 
ts_insert(struct tsymbols *ts, char *key, void *data)
{

  return ERR_SUCCESS;
}
