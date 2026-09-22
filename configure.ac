#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([ale],[1.5],[BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE([subdir-objects])
AM_SILENT_RULES([yes])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/ale.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
# AC_PROG_CC_C11
ALE_AC_PROG_CC_C11

AC_CHECK_LIB([m],[cos])

LT_INIT
AM_PROG_CC_C_O
AC_DEFINE([_POSIX_C_SOURCE], [200809L], [Define the posix version])
AC_DEFINE([_XOPEN_SOURCE], [500], [Define the XOPEN])

# OS detection
case $host in
 *-*-linux*)
	AC_DEFINE([HAVE_LINUX], [], [Defined if you have linux])
	;;
esac

AC_CHECK_FUNCS([getrandom])

AC_RUN_IFELSE([AC_LANG_SOURCE([[
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

]])],[ HAVE_GETRANDOM_SYSCALL="yes"
    AC_DEFINE([HAVE_GETRANDOM_SYSCALL], [], [Defined if you have the linux getrandom syscall])
  
],[],[])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
int main(void)
{
        __builtin_popcountll(10);
        return EXIT_SUCCESS;
}

]])],[ HAVE_POPCOUNTLL="1"
    AC_DEFINE([HAVE_POPCOUNTLL], [], [Defined if you have the popcountll gcc intrinsic])
  
],[],[])


RANDOM_HEADER="#include <sys/random.h>"
RANDOM_FUNC=""
if test "x$ac_cv_func_getrandom" != xyes; then
   RANDOM_HEADER=""
   if test "x$HAVE_GETRANDOM_SYSCALL" == xyes; then
      RANDOM_FUNC="int portability_getrandom_syscall(void *buf, size_t buflen, unsigned int flags);
#define getrandom portability_getrandom_syscall"
   else
      RANDOM_FUNC="#define getrandom portability_getrandom"
   fi
fi

AC_CHECK_FUNCS([strcasestr])

STRCASESTR_FUNC=""
if test "x$ac_cv_func_strcasestr" != xyes; then
   STRCASESTR_FUNC="char* portability_strcasestr(const char *haystack, const char *needle);
#define strcasestr portability_strcasestr"
else
   STRCASESTR_FUNC="char* strcasestr(const char *haystack, const char *needle);"
fi

# qsort_r argument order: POSIX 2024 / glibc / musl / FreeBSD >= 14 use
# qsort_r(base, nmemb, size, compar, arg) with compar(a, b, arg), while
# macOS and older BSDs use qsort_r(base, nmemb, size, arg, compar) with
# compar(arg, a, b). The library is written against the POSIX 2024 order.
# The test programs mimic the library sources (_GNU_SOURCE, no strict
# POSIX level) so that qsort_r is visible on every platform.
m4_define([ALE_QSORT_R_PROLOGUE], [[
#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE
#define _GNU_SOURCE
#include <stdlib.h>
typedef int (*ale_cmp_posix_t)(const void *, const void *, void *);
typedef int (*ale_cmp_bsd_t)(void *, const void *, const void *);
]])

AC_CACHE_CHECK([for POSIX 2024 qsort_r], [ale_cv_func_qsort_r_posix],
  [AC_COMPILE_IFELSE([AC_LANG_SOURCE(ALE_QSORT_R_PROLOGUE[[
_Static_assert(_Generic(&qsort_r,
                        void (*)(void *, size_t, size_t, ale_cmp_posix_t, void *): 1,
                        default: 0),
               "qsort_r does not follow POSIX 2024");
]])], [ale_cv_func_qsort_r_posix=yes], [ale_cv_func_qsort_r_posix=no])])

QSORT_R_FUNC=""
if test "x$ale_cv_func_qsort_r_posix" != xyes; then
   AC_CACHE_CHECK([for BSD qsort_r], [ale_cv_func_qsort_r_bsd],
     [AC_COMPILE_IFELSE([AC_LANG_SOURCE(ALE_QSORT_R_PROLOGUE[[
_Static_assert(_Generic(&qsort_r,
                        void (*)(void *, size_t, size_t, void *, ale_cmp_bsd_t): 1,
                        default: 0),
               "qsort_r does not follow BSD");
]])], [ale_cv_func_qsort_r_bsd=yes], [ale_cv_func_qsort_r_bsd=no])])

   if test "x$ale_cv_func_qsort_r_bsd" != xyes; then
      AC_MSG_ERROR([qsort_r not found (neither POSIX 2024 nor BSD argument order)])
   fi

   AC_DEFINE([HAVE_QSORT_R_BSD], [1], [Defined if qsort_r uses the BSD argument order])
   QSORT_R_FUNC="#include <stdlib.h>
void portability_qsort_r(void *base, size_t nmemb, size_t size,
                         int (*compar)(const void *, const void *, void *),
                         void *arg);
#define qsort_r portability_qsort_r"
fi

AC_SUBST(RANDOM_FUNC)
AC_SUBST(RANDOM_HEADER)
AC_SUBST(FADVISE_FUNC)
AC_SUBST(STRCASESTR_FUNC)
AC_SUBST(QSORT_R_FUNC)
# multi-line value, only meant for portability.h, keep it out of Makefiles
AM_SUBST_NOTMAKE(QSORT_R_FUNC)

AC_OPENMP
# Apple clang: the driver rejects -fopenmp but the frontend supports it
# through -Xpreprocessor. The runtime (libomp, omp.h) is not shipped:
# look for Homebrew or MacPorts and check that it compiles and links.
if test "x$ac_cv_prog_c_openmp" = xunsupported; then
case $host in
 *-*-darwin*)
	AC_MSG_CHECKING([for OpenMP with -Xpreprocessor -fopenmp and libomp])
	ale_omp_found=no
	for ale_omp_prefix in "`brew --prefix libomp 2>/dev/null`" /opt/homebrew/opt/libomp /usr/local/opt/libomp; do
	   ale_omp_inc="$ale_omp_prefix/include"
	   ale_omp_lib="$ale_omp_prefix/lib"
	   if test -n "$ale_omp_prefix" && test -f "$ale_omp_inc/omp.h"; then
	      break
	   fi
	done
	# MacPorts layout
	if test ! -f "$ale_omp_inc/omp.h" && test -f /opt/local/include/libomp/omp.h; then
	   ale_omp_inc=/opt/local/include/libomp
	   ale_omp_lib=/opt/local/lib/libomp
	fi
	if test -f "$ale_omp_inc/omp.h"; then
	   ale_save_CFLAGS=$CFLAGS
	   ale_save_LIBS=$LIBS
	   CFLAGS="$CFLAGS -Xpreprocessor -fopenmp -I$ale_omp_inc"
	   LIBS="$LIBS -L$ale_omp_lib -lomp"
	   AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#ifndef _OPENMP
# error OpenMP not enabled
#endif
#include <omp.h>
]], [[
  int n = 0;
#pragma omp parallel reduction(+:n)
  n += omp_get_num_threads() > 0;
  return 0 == n;
]])],[ ale_omp_found=yes ],[])
	   CFLAGS=$ale_save_CFLAGS
	   if test "x$ale_omp_found" = xyes; then
	      OPENMP_CFLAGS="-Xpreprocessor -fopenmp -I$ale_omp_inc"
	   else
	      LIBS=$ale_save_LIBS
	   fi
	fi
	if test "x$ale_omp_found" = xyes; then
	   AC_MSG_RESULT([yes ($ale_omp_lib)])
	else
	   AC_MSG_RESULT([no])
	fi
	;;
esac
fi
AM_CFLAGS="$AM_CFLAGS $OPENMP_CFLAGS"
AC_SUBST([AM_CFLAGS])

## Check size of long long
AC_CHECK_SIZEOF(long long)

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stdlib.h string.h])
AC_CHECK_HEADERS([pthread.h],,[AC_MSG_ERROR(The pthread header is required in order to build the program!)])
## AC_CHECK_HEADERS([lapacke.h],,[AC_MSG_ERROR(The lapacke header is required in order to build the program!)])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_STRERROR_R


# Checks for pthread.
AC_SEARCH_LIBS([pthread_create], [pthread],,[AC_MSG_ERROR([
-------------------------
 libpthread not found
-------------------------])])
dnl AC_SEARCH_LIBS([LAPACKE_dgels], [lapacke],,[AC_MSG_ERROR([
dnl -------------------------
dnl  liblapacke not found
dnl -------------------------])])


AC_CONFIG_FILES([Makefile src/Makefile test/Makefile perf/Makefile doc/Makefile src/ale/portability.h])
AC_OUTPUT
