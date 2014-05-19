#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "error.h"

int
drop_privileges(uid_t uid, gid_t gid)
{
  ERROR_ERRNO_RET( -1 == setregid(gid, gid), -1);
  ERROR_ERRNO_RET( -1 == setreuid(uid, uid), -1);

  return ERR_SUCCESS;
}

static
int
write_pid(char *pidfile)
{
  pid_t pid = getpid();
  FILE *file;

  ERROR_ERRNO_RET( NULL == (file = fopen(pidfile, "w")), -1);

  fprintf(file, "%d", pid);

  ERROR_ERRNO_RET( 0 != fclose(file), -1 );
  
  return ERR_SUCCESS;
}

/* Stevens Advanced Unix Programming in the UNIX Environment */
int
daemon_init(char *pidfile)
{
  pid_t pid;
  int ret;

  pid = fork();
  
  ERROR_ERRNO_RET(pid < 0, -1);

  if (pid != 0)
    exit(EXIT_SUCCESS);

  setsid();
  chdir("/");
  umask(0);

  ERROR_RET( (ret = write_pid(pidfile)) < 0, ret);

  return ERR_SUCCESS;
}
