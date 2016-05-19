#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/skiplist.h"


int
cmp_int(int a, int b)
{
  return a - b;
}

SKL_INIT(int,16,int,int,cmp_int)

int
main(int argc, char *argv[argc])
{
  struct skl_int skl;
  int val;

  skl_int_init(&skl);
  skl_int_insert(&skl, 1, 1);
  ERROR_UNDEF_FATAL_FMT(1 != skl.header.forward[0]->value,
			"FATAL: 1 != %d\n", skl.header.forward[0]->value);
  ERROR_UNDEF_FATAL(NULL != skl.header.forward[0]->forward[0],
		    "FATAL: NULL != skl.header.forward[0]->forward[0]\n");
  
  skl_int_search(&skl,1,&val);
  ERROR_UNDEF_FATAL_FMT(1 != val,
			"FATAL: skl_int_search() 1 != %d\n", val);

  for (int i = 0 ; i < 10 ; i++)
    skl_int_insert(&skl, i, i);

  for (int i = 0 ; i < 10 ; i++)
    {
      int ret = skl_int_search(&skl,i,&val);
      ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: skl_int_search() %d != 1\n", ret);
      ERROR_UNDEF_FATAL_FMT(i != val,
			    "FATAL: skl_int_search() %d != %d\n", i, val);
    }
			
  
  return EXIT_SUCCESS;
}
