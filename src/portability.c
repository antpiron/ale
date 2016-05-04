#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "ale/portability.h"
#include "ale/error.h"
#include "ale/siphash24.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int 
portability_posix_fadvise(int fd, off_t offset, off_t len, int advice)
{
  

  return 0;
}
#pragma GCC diagnostic pop


static void*
memxor (void *restrict dest, const void *restrict src, size_t n)
{
  const char *s = src;
  char *d = dest;

  for (; n > 0; n--)
    *d++ ^= *s++;

  return dest;
}

// TODO: find better and PROVEN method to generate entropy
static int
gen_key(uint8_t *key)
{
  pid_t pid = getpid();
  pid_t ppid = getppid();
  gid_t gid = getgid();
  uint8_t sip[SIP_HASHLEN];
  struct timeval tv;
  const long avg = (SIP_HASHLEN+SIP_KEYLEN)/2;
  const long half = SIP_HASHLEN/2;

  ERROR_ERRNO_FATAL(-1 == gettimeofday(&tv, NULL),
		    "gettimeofday() failed in gen_key() from module portability\n");

  siphash(sip, (uint8_t*) &pid, sizeof(pid), key);
  memcpy(key, sip, SIP_HASHLEN);

  siphash(sip, (uint8_t*) &gid, sizeof(gid), key);
  memcpy(key+SIP_KEYLEN-SIP_HASHLEN, sip, SIP_HASHLEN);

  siphash(sip, (uint8_t*) &ppid, sizeof(ppid), key);
  memxor(key+avg-SIP_HASHLEN, sip, SIP_HASHLEN);
  
  siphash(sip, (uint8_t*) &tv.tv_usec, sizeof(tv.tv_usec), key);
  memxor(key, sip, half);
  memxor(key+SIP_KEYLEN-half, sip+half, half);
  
  return 0;
}

// Counter mode generator based on siphash
int
portability_getrandom(void *buf, size_t buflen, unsigned int flags)
{
  uint8_t *ubuf = buf;
  static unsigned long counter = 0;
  uint8_t sip[SIP_HASHLEN];
  static uint8_t key[SIP_KEYLEN] = {0};
  
  if (0 == counter)
    gen_key(key);

  for (size_t i = 0 ; i < buflen ; i += SIP_HASHLEN)
    {
      size_t min = buflen-i;
      if (SIP_HASHLEN < min)
	min = SIP_HASHLEN;
      
      siphash(sip, (uint8_t*) &counter, sizeof(counter), key);
      memcpy(ubuf+i, sip, min);
       
      counter++;
    }
  
  return 0;  
}

#ifdef HAVE_GETRANDOM_SYSCALL
#include <linux/random.h>
#include <sys/syscall.h>
int
portability_getrandom_syscall(void *buf, size_t buflen, unsigned int flags)
{
  return syscall(SYS_getrandom, buflen, sizeof(buf), 0);
}
#endif
