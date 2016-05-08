#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>

#include "ale/error.h"
#include "ale/fileutils.h"


int
rmpath(const char *dname, const char *pathname)
{
  size_t dlen = strlen(dname);
  size_t plen = strlen(pathname);
  char fullpath[dlen+plen+2];
  char path_copy[plen + 1];
  char *next_dir;
  
  SUCCESS_RET( 0 ==  strcmp(pathname, ".") || 0 == strcmp(pathname, "/") );

  memcpy(fullpath, dname, dlen);
  fullpath[dlen] = '/';
  memcpy(fullpath + dlen + 1, pathname, plen + 1); 

  ERROR_ERRNO_RET( -1 == rmdir(fullpath), -1);

  strcpy(path_copy, pathname);
  next_dir = dirname(path_copy);

  ERROR_RET(-1 == rmpath(dname, next_dir), -1);
 
  return ERR_SUCCESS;
}

int
unlink_path(const char *dname, const char *pathname)
{
  size_t dlen = strlen(dname);
  size_t plen = strlen(pathname);
  char fullpath[dlen+plen+2];
  char path_copy[plen + 1];
  char *next_dir;
  int ret;

  memcpy(fullpath, dname, dlen);
  fullpath[dlen] = '/';
  memcpy(fullpath + dlen + 1, pathname, plen + 1); 

  ERROR_ERRNO_RET( -1 == unlink(fullpath), -1);

  strcpy(path_copy, pathname);
  next_dir = dirname(path_copy);

  ret = rmpath(dname, next_dir);
  SUCCESS_RET(-1 == ret && error.type == ERR_ERRNO && ENOTEMPTY == errno);
  ERROR_RET(-1 == ret, -1);

  return ERR_SUCCESS;
}

int
mkpath(const char *pathname, mode_t mode)
{
  char path_copy[strlen(pathname)+1];
  char *next_dir;

  SUCCESS_RET( 0 ==  strcmp(pathname, ".") || 0 == strcmp(pathname, "/") );

  SUCCESS_RET( 0 == access(pathname, F_OK) );

  strcpy(path_copy, pathname);
  next_dir = dirname(path_copy);

  ERROR_RET(-1 == mkpath(next_dir, mode) && EEXIST != errno, -1);

  return mkdir(pathname, mode);
}

FILE*
mkpath_fopen(const char *pathname, const char *mode)
{
  char path_copy[strlen(pathname)+1];
  char *next_dir;
  mode_t mode_dir = S_IRWXU;
  FILE *file;
  int fd;
  int flags = O_RDWR;

  strcpy(path_copy, pathname);
  next_dir = dirname(path_copy);
  
  ERROR_RET(-1 == mkpath(next_dir, mode_dir), NULL);

  if ('w' == mode[0] || 'a' == mode[0])
    flags |= O_CREAT;
  if (strchr(mode, 'x'))
    flags |= O_EXCL;

  ERROR_ERRNO_RET( -1 == (fd = open(pathname, flags, S_IRUSR|S_IWUSR)), NULL);
  ERROR_ERRNO_RET( NULL == (file = fdopen(fd, mode)), (close(fd),NULL));
  
  return file; // TODO: if fopen fail, empty directories may be present
}

/*
static char*
read_unescaped(FILE *file)
{
}

static char*
read_escaped(FILE *file)
{
}

static inline int
match(FILE *file, unsigned char c)
{
  unsigned char rc = fgetc(file);

  ERROR_FERROR_RET(EOF == rc, -1);
  ERROR_UNDEF_RET(1, -1);

  return 0;
}

intptr_t
read_record(FILE *file, char **strings, size_t n)
{
  size_t count = 0;
  strings[count++] = read_field(FILE *file);;
  unsigned char c = fgetc(file);
  ERROR_FERROR_RET(EOF == c, -1);
  match()
}


intptr_t
csv_readline(FILE *file, char **strings, size_t n)
{
  intptr_t count = read_record(file, strings, n);
  ERROR_RET(-1 == count);
  
  if (feof(file))
    return count;

  unsigned char c = fgetc(file);
  ERROR_FERROR_RET(EOF == c, -1);

  if ('\r' == c)
    {
      unsigned char c = fgetc(file);
      ERROR_FERROR_RET(EOF == c, -1);
    }

  ERROR_UNDEF('\n' != c);
    

  return count;
}

int
csv_init(struct csv *csv, FILE *file)
{
  csv->file = file;
  
  return 0;
}
void csv_destroy(struct csv *csv);
intptr_t csv_readline(struct csv *csv);
*/
