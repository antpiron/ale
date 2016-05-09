#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>

#include "ale/error.h"
#include "ale/fileutils.h"

#include "csv_tokens.h"

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

int
csv_init(FILE *file)
{
  csvin = file;

  return 0;
}

int
csv_readline(struct sl_node *node)
{
  size_t fieldnum = 0;
  struct sl_node *last = node;
  int token = csvlex();

  while (CSV_EOL == token)
    token = csvlex();

  if (CSV_EOF == token)
    return 0;

  while (1)
    {
      if (CSV_FIELD == token)
        {
	  sl_push(last, strdup(csvlval));
	  last = last->next;
	  fieldnum++;
	  
	  token = csvlex();

	  if (CSV_EOF == token || CSV_EOL == token)
	    return fieldnum;

	  ERROR_UNDEF_RET(CSV_COMMA != token, -1);
	}
      else if (CSV_COMMA == token)
	{
	  sl_push(last, strdup(csvlval));
	  last = last->next;
	  fieldnum++;
	}

      token = csvlex();
    }

  return fieldnum;
}
