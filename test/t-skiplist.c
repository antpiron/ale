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

  
  return EXIT_SUCCESS;
}
