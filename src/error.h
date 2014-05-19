#ifndef __ERROR_H
#define __ERROR_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>

#ifndef LOG_PERROR
# define LOG_PERROR 0
#endif

#define ERR_SUCCESS 0
#define ERR_ERRNO  -1
#define ERR_RPC    -2
#define ERR_FERROR -3
#define ERR_GAI    -4
#define ERR_UNDEF  -5
#define ERR_ANY    -16
#define ERR_LAST   ERR_ANY

int* error_get_errno();

#define error_errno (*error_get_errno())

#define ERROR_RET(test,val)			\
  do						\
    {						\
      if (test)					\
	  return (val);				\
    }						\
  while (0)

#define SUCCESS_RET(test)						\
  do									\
    {									\
      if (test)								\
	{								\
	  return ERR_SUCCESS;						\
	}								\
    }									\
  while (0)


#define ERROR_TYPE_RET(test,err,val)		\
  do						\
    {						\
      if (test)					\
	{					\
	  error_errno = (err);			\
	  return (val);				\
	}					\
    }						\
  while (0)

#define ERROR_ERRNO_RET(test,val) ERROR_TYPE_RET((test),ERR_ERRNO,(val))
#define ERROR_GAI_RET(test,val) ERROR_TYPE_RET((test),ERR_GAI,(val))
#define ERROR_FERROR_RET(test,val) ERROR_TYPE_RET((test),ERR_FERROR,(val))
#define ERROR_UNDEF_RET(test,val) ERROR_TYPE_RET((test),ERR_UNDEF,(val))


#define ERROR_PRINTF(fmt,...)						\
  do									\
    {									\
      fprintf(stderr, fmt, __VA_ARGS__);				\
      if (ERR_ERRNO == error_errno)					\
	fprintf(stderr, "%s\n", strerror(errno)); 			\
      else if (ERR_FERROR == error_errno)				\
	fprintf(stderr, "%s\n", "file error\n");			\
      else if (ERR_GAI == error_errno)					\
	fprintf(stderr, "%s\n", "gai error\n");				\
      else if (ERR_UNDEF == error_errno)				\
	fprintf(stderr, "%s\n", "undefined error\n");			\
    }									\
  while (0)
  

#define ERROR_FATAL_FMT(test,fmt,...)		\
  do						\
    {						\
      if (test)					\
	{					\
	  ERROR_PRINTF(fmt, __VA_ARGS__);	\
	  exit(EXIT_FAILURE);			\
	}					\
    }						\
  while (0)

#define ERROR_FATAL(test,str) ERROR_FATAL_FMT((test),"%s",(str))

#define ERROR_TYPE_FATAL_FMT(test,err,fmt,...)	\
  do						\
    {						\
      if (test)					\
	{					\
	  error_errno = (err);			\
	  ERROR_FATAL_FMT(1,fmt, __VA_ARGS__);	\
	}					\
    }						\
  while (0)

#define ERROR_TYPE_FATAL(test,err,str) ERROR_TYPE_FATAL_FMT((test),(err),"%s",(str))

#define ERROR_ERRNO_FATAL_FMT(test,fmt,...)  ERROR_TYPE_FATAL_FMT((test),ERR_ERRNO,fmt, __VA_ARGS__)
#define ERROR_GAI_FATAL_FMT(test,fmt,...)    ERROR_TYPE_FATAL_FMT((test),ERR_GAI,fmt, __VA_ARGS__)
#define ERROR_FERROR_FATAL_FMT(test,fmt,...) ERROR_TYPE_FATAL_FMT((test),ERR_FERROR,fmt, __VA_ARGS__)
#define ERROR_UNDEF_FATAL_FMT(test,fmt,...)  ERROR_TYPE_FATAL_FMT((test),ERR_UNDEF,fmt, __VA_ARGS__)

#define ERROR_ERRNO_FATAL(test,str,...)  ERROR_TYPE_FATAL((test),ERR_ERRNO,str)
#define ERROR_GAI_FATAL(test,str,...)    ERROR_TYPE_FATAL((test),ERR_GAI,str)
#define ERROR_FERROR_FATAL(test,str,...) ERROR_TYPE_FATAL((test),ERR_FERROR,str)
#define ERROR_UNDEF_FATAL(test,str,...)  ERROR_TYPE_FATAL((test),ERR_UNDEF,str)

#define ERROR_SYSLOG(level,fmt,...)					\
  do									\
    {									\
      int lev = (level);						\
      if (ERR_ERRNO == error_errno)					\
	syslog(lev, fmt": %s", __VA_ARGS__, strerror(errno));		\
      else if (ERR_FERROR == error_errno)				\
	syslog(lev, fmt"%s\n", __VA_ARGS__, "file error\n");		\
      else if (ERR_GAI == error_errno)					\
	syslog(lev, fmt"%s\n", __VA_ARGS__, "gai error\n");		\
      else if (ERR_UNDEF == error_errno)				\
	syslog(lev, fmt"%s\n", __VA_ARGS__, "undefined error\n");	\
    }									\
  while (0)

#define ERROR_LOG_FMT(test,level,fmt,...)				\
  do									\
    {									\
      if (test)								\
	ERROR_SYSLOG((level), fmt, __VA_ARGS__);			\
    } while (0)


#define ERROR_LOG(test,level,str) ERROR_LOG_FMT((test),(level),"%s",(str))


#endif
