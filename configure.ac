#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([ale], [1.0], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/ale.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
# AC_PROG_CC_C11
ALE_AC_PROG_CC_C11
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
AC_DEFINE([_POSIX_C_SOURCE], [200809L], [Define the posix version])
AC_DEFINE([_XOPEN_SOURCE], [500], [Define the XOPEN])

# OS detection
case $host in
 *-*-linux*)
	AC_DEFINE([HAVE_LINUX], [], [Defined if you have linux])
	;;
esac

AC_TRY_RUN([
#include <stdlib.h>
#include <unistd.h>
#include <linux/random.h>
#include <sys/syscall.h>

int main(void)
{
        char buf[10];
        long r = syscall(SYS_getrandom, buf, sizeof(buf), 0);
	if (-1 == r)
	   return EXIT_FAILURE;
        return EXIT_SUCCESS;
}

]
, AC_DEFINE([HAVE_GETRANDOM_SYSCALL], [], [Defined if you have the linux getrandom syscall])
, [ echo "little endian" ]
)


# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stdlib.h string.h])
AC_CHECK_HEADERS([pthread.h],,[AC_MSG_ERROR(The pthread header is required in order to build the program!)])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([getrandom])
AC_FUNC_STRERROR_R

# Checks for pthread.
AC_SEARCH_LIBS([pthread_create], [pthread],,[AC_MSG_ERROR([
-------------------------
 libpthread not found
-------------------------])])


AC_CONFIG_FILES([Makefile src/Makefile test/Makefile doc/Makefile])
AC_OUTPUT
