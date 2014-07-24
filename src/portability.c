#include "portability.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int 
portability_posix_fadvise(int fd, off_t offset, off_t len, int advice)
{
  

  return 0;
}
#pragma GCC diagnostic pop
