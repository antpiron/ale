#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "error.h"
#include "fileutils.h"

char *paths[] = {"/tmp/alefs-test/test/test", NULL};

int
main(int argc, char *argv[argc])
{
  for (char **path = paths ; *path ; path++)
    {
      ERROR_FATAL_FMT(-1 == mkpath(*path, S_IRWXU), "FAIL: %s\n", *path);
      ERROR_ERRNO_FATAL(-1 == access(*path, R_OK|W_OK|X_OK), "FAIL: access refused\n");
    }
            
  return EXIT_SUCCESS;
}
