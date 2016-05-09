#ifndef __FILEUTILS_H
#define __FILEUTILS_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <ale/sl_list.h>

int rmpath(const char *dname, const char *pathname);
int unlink_path(const char *dname, const char *pathname);
int mkpath(const char *pathname, mode_t mode);
FILE* mkpath_fopen(const char *pathname, const char *mode);

int csv_init(FILE *file);
int csv_readline(struct sl_node *node);


#endif
