#ifndef __PORTABILITY_H
#define __PORTABILITY_H

#include <limits.h>
#include <netdb.h>

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#if defined(__linux__)
#  include <endian.h>
#elif defined(__FreeBSD__) || defined(__NetBSD__)
#  include <sys/endian.h>
#elif defined(__OpenBSD__)
#  include <sys/types.h>
#  define be16toh(x) betoh16(x)
#  define be32toh(x) betoh32(x)
#  define be64toh(x) betoh64(x)
#endif

#define htonll(x) htobe64(x)
#define ntohll(x) be64toh(x)

// (Not in POSIX.1-2001.)  Print to stderr as well.
#ifndef LOG_PERROR
# define LOG_PERROR 0
#endif

#endif
