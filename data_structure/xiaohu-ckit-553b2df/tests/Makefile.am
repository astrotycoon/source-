##                                       -*- Automake -*-
## Process this file with automake to produce Makefile.in
##
AM_CPPFLAGS = -DCKIT_COMPILATION -DDMALLOC_FUNC_CHECK \
	      -I$(top_srcdir) @CHECK_CFLAGS@

check_PROGRAMS = ckit-check
noinst_PROGRAMS = ckit-basex

ckit_basex_SOURCES = ckit-basex.c
ckit_basex_LDADD   = $(top_builddir)/ckit/libckit.la

ckit_check_SOURCES = ckit-check.c
ckit_check_LDFLAGS =
ckit_check_LDADD   = $(top_builddir)/ckit/libckit.la @CHECK_LIBS@

TESTS = ckit-check

CLEANFILES = *~ *.gcno *.gcda *.gcov
DISTCLEANFILES =
