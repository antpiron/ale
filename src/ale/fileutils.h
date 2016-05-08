#ifndef __FILEUTILS_H
#define __FILEUTILS_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int rmpath(const char *dname, const char *pathname);
int unlink_path(const char *dname, const char *pathname);
int mkpath(const char *pathname, mode_t mode);
FILE* mkpath_fopen(const char *pathname, const char *mode);

/*
struct csv
{
  FILE *file;
  struct hash fields;
};
  
int csv_init(struct csv *csv, FILE *file);
void csv_destroy(struct csv *csv);
intptr_t csv_readline(struct csv *csv);
*/

#endif
