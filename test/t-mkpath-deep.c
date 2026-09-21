#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/fileutils.h"

// Regression: dirname() static buffer (macOS/BSD) clobbered paths in recursion

#define DEEP "a/b/c/d/e"

int
main(int argc, char *argv[argc])
{
  char base[] = "/tmp/ale-test-mkpath-deep-XXXXXX";
  char path[sizeof(base) + 64];
  FILE *file;

  ERROR_ERRNO_FATAL(NULL == mkdtemp(base), "FAIL: mkdtemp\n");

  snprintf(path, sizeof(path), "%s/" DEEP, base);
  ERROR_FATAL_FMT(-1 == mkpath(path, S_IRWXU), "FAIL: mkpath(\"%s\")\n", path);
  ERROR_ERRNO_FATAL_FMT(-1 == access(path, R_OK|W_OK|X_OK), "FAIL: access(\"%s\")\n", path);

  snprintf(path, sizeof(path), "%s/f/g/h/i/file", base);
  ERROR_FATAL_FMT(NULL == (file = mkpath_fopen(path, "w")), "FAIL: mkpath_fopen(\"%s\")\n", path);
  ERROR_ERRNO_FATAL(-1 == fclose(file), "FAIL: fclose\n");
  ERROR_ERRNO_FATAL_FMT(-1 == access(path, F_OK|W_OK), "FAIL: access(\"%s\")\n", path);

  ERROR_FATAL(-1 == unlink_path(base, "f/g/h/i/file"), "FAIL: unlink_path(\"f/g/h/i/file\")\n");
  snprintf(path, sizeof(path), "%s/f", base);
  ERROR_UNDEF_FATAL_FMT(0 == access(path, F_OK), "FAIL: \"%s\" not removed\n", path);

  ERROR_FATAL(-1 == rmpath(base, DEEP), "FAIL: rmpath(\"" DEEP "\")\n");
  snprintf(path, sizeof(path), "%s/a", base);
  ERROR_UNDEF_FATAL_FMT(0 == access(path, F_OK), "FAIL: \"%s\" not removed\n", path);

  // dirname("") == "." is longer than ""
  ERROR_UNDEF_FATAL(-1 != mkpath("", S_IRWXU), "FAIL: mkpath(\"\") succeeded\n");

  ERROR_ERRNO_FATAL_FMT(-1 == rmdir(base), "FAIL: rmdir(\"%s\")\n", base);

  return EXIT_SUCCESS;
}
