##                                       -*- Automake -*-
## Process this file with automake to produce Makefile.in
##
AM_CPPFLAGS = -DCKIT_COMPILATION -DDMALLOC_FUNC_CHECK \
	      -I$(top_srcdir)

sources = ckit-version.c ckit-log.c ckit-list.c ckit-crc.c
sources+= ckit-base64.c ckit-sort.c ckit-node.c ckit-hash.c
sources+= ckit-slab.c
headers = ckit-version.h ckit-log.h ckit-list.h ckit-crc.h
headers+= ckit-base64.h ckit-sort.h ckit-node.h ckit-hash.h
headers+= ckit-slab.h
headers+= ckit.h ckit-config.h ckit-macros.h ckit-types.h
pri_headers =

lib_LTLIBRARIES = libckit.la
libckit_la_SOURCES = $(sources) $(headers) $(pri_headers)
libckit_la_LDFLAGS =
libckit_la_LIBADD  =

pkginclude_HEADERS = $(headers)

subst = sed -e "s,[@]VERSION[@],$(VERSION),g" \
	-e "s,[@]prefix[@],$(prefix),g" \
	-e "s,[@]exec_prefix[@],$(exec_prefix),g" \
	-e "s,[@]libdir[@],$(libdir),g" \
	-e "s,[@]includedir[@],$(includedir),g"

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA = ckit.pc
ckit.pc: ckit.pc.in Makefile
	$(subst) < $(srcdir)/ckit.pc.in > $@

EXTRA_DIST = ckit.pc.in

CLEANFILES = $(srcdir)/*~
DISTCLEANFILES = ckit-config.h ckit.pc
