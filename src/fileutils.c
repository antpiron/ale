#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>

#include "ale/error.h"
#include "ale/fileutils.h"
#include "ale/process.h"


/* dirname() may return a pointer to a static buffer (macOS, BSD) that the
   next call overwrites: copy the result back into path to make it reentrant.
   path must have room for at least 2 bytes, dirname("") is "." */
static char *
path_dirname(char *path)
{
  char *dir = dirname(path);

  memmove(path, dir, strlen(dir) + 1);

  return path;
}

int
rmpath(const char *dname, const char *pathname)
{
  size_t dlen = strlen(dname);
  size_t plen = strlen(pathname);
  char fullpath[dlen+plen+2];
  // +2: NUL and path_dirname("") == "."
  char path_copy[plen + 2];
  char *next_dir;
  
  SUCCESS_RET( 0 ==  strcmp(pathname, ".") || 0 == strcmp(pathname, "/") );

  memcpy(fullpath, dname, dlen);
  fullpath[dlen] = '/';
  memcpy(fullpath + dlen + 1, pathname, plen + 1); 

  ERROR_ERRNO_RET( -1 == rmdir(fullpath), -1);

  strcpy(path_copy, pathname);
  next_dir = path_dirname(path_copy);

  ERROR_RET(-1 == rmpath(dname, next_dir), -1);
 
  return ERR_SUCCESS;
}

int
unlink_path(const char *dname, const char *pathname)
{
  size_t dlen = strlen(dname);
  size_t plen = strlen(pathname);
  char fullpath[dlen+plen+2];
  // +2: NUL and path_dirname("") == "."
  char path_copy[plen + 2];
  char *next_dir;
  int ret;

  memcpy(fullpath, dname, dlen);
  fullpath[dlen] = '/';
  memcpy(fullpath + dlen + 1, pathname, plen + 1); 

  ERROR_ERRNO_RET( -1 == unlink(fullpath), -1);

  strcpy(path_copy, pathname);
  next_dir = path_dirname(path_copy);

  ret = rmpath(dname, next_dir);
  SUCCESS_RET(-1 == ret && ale_error.type == ERR_ERRNO && ENOTEMPTY == errno);
  ERROR_RET(-1 == ret, -1);

  return ERR_SUCCESS;
}

int
mkpath(const char *pathname, mode_t mode)
{
  // +2: NUL and path_dirname("") == "."
  char path_copy[strlen(pathname)+2];
  char *next_dir;

  SUCCESS_RET( 0 ==  strcmp(pathname, ".") || 0 == strcmp(pathname, "/") );

  SUCCESS_RET( 0 == access(pathname, F_OK) );

  strcpy(path_copy, pathname);
  next_dir = path_dirname(path_copy);

  ERROR_RET(-1 == mkpath(next_dir, mode) && EEXIST != errno, -1);

  // EEXIST: created concurrently since access()
  ERROR_ERRNO_RET(-1 == mkdir(pathname, mode) && EEXIST != errno, -1);
  
  return 0;
}

FILE*
mkpath_fopen(const char *pathname, const char *mode)
{
  // +2: NUL and path_dirname("") == "."
  char path_copy[strlen(pathname)+2];
  char *next_dir;
  mode_t mode_dir = S_IRWXU;
  FILE *file;
  int fd;
  int flags = O_RDWR;

  strcpy(path_copy, pathname);
  next_dir = path_dirname(path_copy);
  
  ERROR_RET(-1 == mkpath(next_dir, mode_dir), NULL);

  if ('w' == mode[0] || 'a' == mode[0])
    flags |= O_CREAT;
  if (strchr(mode, 'x'))
    flags |= O_EXCL;

  ERROR_ERRNO_RET( -1 == (fd = open(pathname, flags, S_IRUSR|S_IWUSR)), NULL);
  ERROR_ERRNO_RET( NULL == (file = fdopen(fd, mode)), (close(fd),NULL));
  
  return file; // TODO: if fopen fail, empty directories may be present
}

int
is_gzip(const char *filename)
{
  FILE *file;
  const size_t header_size = 2;
  unsigned char id[header_size];
  int ret = 0;

  file = fopen(filename, "r");
  if (NULL != file)
    {      
      size_t nread = fread(id, 1, header_size, file);
      if ( 2 == nread && 0x1f == id[0] && 0x8b == id[1] )
	ret = 1;
      
      fclose(file);
    }
  
  return ret;
}

FILE*
gzfopen(const char *pathname, const char *mode)
{
  if (0 != strcmp("r", mode))
    {
      errno = EINVAL;
      ERROR_ERRNO_RET(1, NULL);
    }

  return process_popenp("gunzip", "gunzip", "--stdout", pathname, NULL);
}

