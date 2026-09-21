#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ale/error.h"
#include "ale/fileutils.h"

// Regression: concurrent mkpath failed with EEXIST between access() and mkdir()

#define NPROC (16)
#define ROUNDS (32)

int
main(int argc, char *argv[argc])
{
  char base[] = "/tmp/ale-test-mkpath-concurrent-XXXXXX";
  char path[sizeof(base) + 64];
  char rel[64];

  ERROR_ERRNO_FATAL(NULL == mkdtemp(base), "FAIL: mkdtemp\n");

  for (int round = 0 ; round < ROUNDS ; round++)
    {
      int start[2];
      pid_t pids[NPROC];
      int failed = 0;

      snprintf(rel, sizeof(rel), "r%d/a/b/c", round);
      snprintf(path, sizeof(path), "%s/%s", base, rel);

      // children start together when the parent closes the pipe
      ERROR_ERRNO_FATAL(-1 == pipe(start), "FAIL: pipe\n");

      for (int i = 0 ; i < NPROC ; i++)
	{
	  ERROR_ERRNO_FATAL(-1 == (pids[i] = fork()), "FAIL: fork\n");
	  if (0 == pids[i])
	    {
	      char c;

	      close(start[1]);
	      (void) read(start[0], &c, 1);
	      _exit(-1 == mkpath(path, S_IRWXU) ? EXIT_FAILURE : EXIT_SUCCESS);
	    }
	}

      close(start[0]);
      close(start[1]);

      for (int i = 0 ; i < NPROC ; i++)
	{
	  int status;

	  ERROR_ERRNO_FATAL(-1 == waitpid(pids[i], &status, 0), "FAIL: waitpid\n");
	  failed += !WIFEXITED(status) || EXIT_SUCCESS != WEXITSTATUS(status);
	}

      ERROR_UNDEF_FATAL_FMT(failed, "FAIL: round %d, %d/%d concurrent mkpath(\"%s\") failed\n",
			    round, failed, NPROC, path);
      ERROR_ERRNO_FATAL_FMT(-1 == access(path, F_OK), "FAIL: access(\"%s\")\n", path);
      ERROR_FATAL_FMT(-1 == rmpath(base, rel), "FAIL: rmpath(\"%s\")\n", rel);
    }

  ERROR_ERRNO_FATAL_FMT(-1 == rmdir(base), "FAIL: rmdir(\"%s\")\n", base);

  return EXIT_SUCCESS;
}
