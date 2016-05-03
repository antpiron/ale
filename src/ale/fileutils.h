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

#endif
