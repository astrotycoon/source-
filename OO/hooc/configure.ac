#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([HandyObjectOrientedC], [0.0], [fourdollars@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2 foreign])
AC_CONFIG_SRCDIR([logger.h])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_VPRINTF

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
