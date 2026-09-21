// macOS hides qsort_r when _POSIX_C_SOURCE is defined (config.h)
#define _DARWIN_C_SOURCE

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <ctype.h>
#include <stdatomic.h>

#include "ale/portability.h"
#include "ale/error.h"
#include "ale/siphash24.h"
#include "ale/random.h"


int 
portability_posix_fadvise(int fd, off_t offset, off_t len, int advice)
{
  (void) (fd);
  (void) (offset);
  (void) (len);
  (void) (advice);

  return 0;
}


int
portability_getrandom(void *buf, size_t buflen, unsigned int flags)
{
  return rd_getrandom(buf, buflen, flags);
}

#ifdef HAVE_GETRANDOM_SYSCALL
#include <linux/random.h>
#include <sys/syscall.h>
// Where is this fucking declaration in ubuntu???
long syscall(long number, ...);
int
portability_getrandom_syscall(void *buf, size_t buflen, unsigned int flags)
{
  return syscall(SYS_getrandom, buf, buflen, flags);
}
#endif

char*
portability_strcasestr(const char *haystack, const char *needle)
{
  char *result = NULL;
  // TODO: use Knuth–Morris–Pratt algorithm
  for (char *h = (char*) haystack ; *h ; h++)
    {
      char *n = (char*) needle;
      for (char *hh = (char*) h ; *n && *hh ; n++, hh++)
	if ( tolower(*hh) != tolower(*n) )
	  break;
      
      if ('\0' == *n)
	{
	  result = h;
	  break;
	}
    }

  return result;
}


#ifdef HAVE_QSORT_R_BSD
// portability.h redirects qsort_r to portability_qsort_r, use the libc one here
#undef qsort_r

struct portability_qsort_r_closure
{
  int (*compar)(const void *, const void *, void *);
  void *arg;
};

static int
portability_qsort_r_compar(void *arg, const void *p1, const void *p2)
{
  struct portability_qsort_r_closure *closure = arg;

  return closure->compar(p1, p2, closure->arg);
}

void
portability_qsort_r(void *base, size_t nmemb, size_t size,
		    int (*compar)(const void *, const void *, void *),
		    void *arg)
{
  struct portability_qsort_r_closure closure = {.compar = compar, .arg = arg};

  qsort_r(base, nmemb, size, &closure, portability_qsort_r_compar);
}
#endif
