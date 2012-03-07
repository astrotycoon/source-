libc_add_ons = nptl $(add-ons)
libc_extra_cflags = -O3
libc_rtlddir = /lib64
libc_extra_config_options = $(extra_config_options) --enable-multi-arch

define libc6_extra_pkg_install

make -C debian/local/memcpy-wrapper
install -m 755 -o root -g root -d debian/libc6/$(libdir)/libc
install -m 755 -o root -g root \
	debian/local/memcpy-wrapper/memcpy-preload.so \
	debian/libc6/$(libdir)/libc
install -m 755 -o root -g root \
	debian/local/memcpy-wrapper/memcpy-syslog-preload.so \
	debian/libc6/$(libdir)/libc
endef

# build 32-bit (i386) alternative library
EGLIBC_PASSES += i386
DEB_ARCH_REGULAR_PACKAGES += libc6-i386 libc6-dev-i386
libc6-i386_shlib_dep = libc6-i386 (>= $(shlib_dep_ver))

# This section is quite different in Ubuntu.
#
# This only looks like i386.  It's really i686, fine for compatability
i386_add-ons = nptl $(add-ons)
i386_configure_target = i686-linux-gnu
i386_CC = $(BUILD_CC) -m32
i386_CXX = $(BUILD_CXX) -m32
i386_MAKEFLAGS = MAKEFLAGS="gconvdir=/usr/lib32/gconv"
i386_extra_cflags = -march=i686 -mtune=generic
i386_extra_config_options = $(extra_config_options) --disable-profile --enable-multi-arch
i386_slibdir = /lib32
i386_libdir = /usr/lib32

define libc6-dev-i386_extra_pkg_install

mkdir -p debian/libc6-dev-i386/usr/include
ln -s x86_64-linux-gnu/bits debian/libc6-dev-i386/usr/include/
ln -s x86_64-linux-gnu/gnu debian/libc6-dev-i386/usr/include/
ln -s x86_64-linux-gnu/fpu_control.h debian/libc6-dev-i386/usr/include/

mkdir -p debian/libc6-dev-i386/usr/include/x86_64-linux-gnu/gnu
cp -a debian/tmp-i386/usr/include/gnu/stubs-32.h \
        debian/libc6-dev-i386/usr/include/x86_64-linux-gnu/gnu

mkdir -p debian/libc6-dev-i386/usr/include/sys
for i in `ls debian/tmp-libc/usr/include/x86_64-linux-gnu/sys` ; do \
	ln -s ../x86_64-linux-gnu/sys/$$i debian/libc6-dev-i386/usr/include/sys/$$i ; \
done

cp -a debian/tmp-i386/usr/include/sys/elf.h \
	debian/libc6-dev-i386/usr/include/sys
cp -a debian/tmp-i386/usr/include/sys/vm86.h \
	debian/libc6-dev-i386/usr/include/sys

endef

define libc6-i386_extra_pkg_install
mkdir -p debian/libc6-i386/lib
ln -sf /lib32/ld-linux.so.2 debian/libc6-i386/lib
endef

