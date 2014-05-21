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
AC_PROG_CC_STDC
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

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
AC_CHECK_FUNCS([])
AC_FUNC_STRERROR_R

# Checks for pthread.
AC_SEARCH_LIBS([pthread_create], [pthread],,[AC_MSG_ERROR([
-------------------------
 libpthread not found
-------------------------])])


AC_CONFIG_FILES([Makefile src/Makefile test/Makefile doc/Makefile])
AC_OUTPUT
