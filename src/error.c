#include <pthread.h>

#include "error.h"

static pthread_key_t errno_key;
static pthread_once_t errno_once = PTHREAD_ONCE_INIT;


static void
error_free(void *ptr)
{
  free(ptr);
  pthread_setspecific(errno_key, NULL);
}

static void 
error_init(void)
{ 
  pthread_key_create(&errno_key, error_free); 
}


int* 
error_get_errno()
{
  int *error = NULL;

  pthread_once(&errno_once, error_init);

  error = pthread_getspecific(errno_key);
  if (!error)
  {
    error = malloc(sizeof(*error));
    pthread_setspecific(errno_key, error);
  }

  return error;
}

