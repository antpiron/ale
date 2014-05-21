#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "error.h"

#define TEST(test,msg)						\
  do								\
    {								\
      if (test)							\
	{							\
	  fprintf(stderr, "%s\n", (msg));			\
	  exit(EXIT_FAILURE);					\
	}							\
    }								\
  while (0)


int
test_error_undef_ret()
{
  ERROR_UNDEF_RET(1, -1);

  return 0;
}

int
test_error_errno_ret()
{
  errno = EACCES;
  ERROR_ERRNO_RET(1, -1);

  return 0;
}

int
test_error_gai_ret()
{
  ERROR_GAI_RET(1, -1);

  return 0;
}

int
test_error_ferror_ret()
{
  ERROR_FERROR_RET(1, -1);

  return 0;
}

int
test_error_ret()
{
  ERROR_RET(-1 == test_error_errno_ret(), -1);
  
  return 0;
}

int
main(int argc, char *argv[argc])
{

  TEST(-1 != test_error_undef_ret() || error_errno != ERR_UNDEF, "FAIL: ERROR_UNDEF_RET()");
  TEST(-1 != test_error_errno_ret() || error_errno != ERR_ERRNO || errno != EACCES, "FAIL: ERROR_ERRNO_RET()");
  TEST(-1 != test_error_gai_ret() || error_errno != ERR_GAI, "FAIL: ERROR_GAI_RET()");
  TEST(-1 != test_error_ferror_ret() || error_errno != ERR_FERROR, "FAIL: ERROR_FERROR_RET()");

  TEST(-1 != test_error_ret() || error_errno != ERR_ERRNO || errno != EACCES, "FAIL: ERROR_RET()");

  return EXIT_SUCCESS;
}
