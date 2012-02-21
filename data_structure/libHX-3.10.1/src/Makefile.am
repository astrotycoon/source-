# -*- Makefile -*-

AM_CPPFLAGS = ${regular_CPPFLAGS} -I${top_srcdir}/include
AM_CFLAGS   = ${regular_CFLAGS} ${GCC_FVISIBILITY_HIDDEN}
AM_CXXFLAGS = ${regular_CXXFLAGS} ${GCC_FVISIBILITY_HIDDEN}

lib_LTLIBRARIES  = libHX.la
libHX_la_SOURCES = deque.c dl.c format.c io.c map.c \
                   mc.c misc.c opt.c \
                   rand.c string.c time.c ${mingw32_extra} ${proc_sources}
libHX_la_LIBADD  = ${libdl_LIBS} ${libpthread_LIBS} ${librt_LIBS}
libHX_la_LDFLAGS = -version-info 27:0:0 \
                   -Wl,--version-script=${srcdir}/libHX.map

if MINGW32
mingw32_extra    = ux-file.c ux-mmap.c
endif
if B_PROC
proc_sources     = proc.c
endif

check_PROGRAMS     = tc-compile tc-cast tc-deque tc-dir tc-link \
                     tc-list tc-list2 tc-map tc-misc tc-netio \
                     tc-option tc-proc tc-rand tc-realpath tc-string tc-time
tc_cast_CFLAGS     = ${AM_CFLAGS} -std=c99
tc_cast_LDADD      = libHX.la -lm
tc_compile_LDADD   = libHX.la
tc_dir_LDADD       = libHX.la
tc_link_LDADD      = libHX.la
tc_list_LDADD      = libHX.la
tc_list2_LDADD     = libHX.la
tc_list2_CFLAGS    = ${AM_CFLAGS} -O2 -fstrict-aliasing
tc_map_LDADD       = libHX.la -lm
tc_misc_LDADD      = libHX.la
tc_netio_LDADD     = libHX.la ${libsocket_LIBS}
tc_option_LDADD    = libHX.la
tc_proc_LDADD      = libHX.la
tc_rand_LDADD      = libHX.la
tc_realpath_LDADD  = libHX.la
tc_string_LDADD    = libHX.la
tc_time_LDADD      = libHX.la

if HAVE_CXX
check_PROGRAMS    += tx-compile tx-cast tx-deque tx-dir tx-list tx-list2 \
                     tx-misc tx-netio \
                     tx-option tx-proc tx-rand tx-string tx-time
tx_cast_SOURCES    = tx-cast.cpp
tx_cast_CXXFLAGS   = ${AM_CXXFLAGS} -std=c++98
tx_cast_LDADD      = libHX.la -lm
tx_compile_SOURCES = tx-compile.cpp
tx_compile_LDADD   = libHX.la
tx_deque_SOURCES   = tx-deque.cpp
tx_dir_SOURCES     = tx-dir.cpp
tx_dir_LDADD       = libHX.la
tx_list_SOURCES    = tx-list.cpp
tx_list_LDADD      = libHX.la
tx_list2_SOURCES   = tx-list2.cpp
tx_list2_CXXFLAGS  = ${AM_CXXFLAGS} -O2 -fstrict-aliasing
tx_list2_LDADD     = libHX.la
tx_misc_SOURCES    = tx-misc.cpp
tx_misc_LDADD      = libHX.la
tx_netio_SOURCES   = tx-netio.cpp
tx_netio_LDADD     = libHX.la ${libsocket_LIBS}
tx_option_SOURCES  = tx-option.cpp
tx_proc_SOURCES    = tx-proc.cpp
tx_proc_LDADD      = libHX.la
tx_rand_SOURCES    = tx-rand.cpp
tx_rand_LDADD      = libHX.la ${librt_LIBS}
tx_string_SOURCES  = tx-string.cpp
tx_string_LDADD    = libHX.la
tx_time_SOURCES    = tx-time.cpp
tx_time_LDADD      = libHX.la ${librt_LIBS}
endif
