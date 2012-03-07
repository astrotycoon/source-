libc_add-ons = ports nptl $(add-ons)

EGLIBC_PASSES += armhf
DEB_ARCH_REGULAR_PACKAGES += libc6-armhf libc6-dev-armhf
armhf_add-ons = ports nptl $(add-ons)
armhf_CC = $(BUILD_CC) -mfloat-abi=hard
armhf_CXX = $(BUILD_CXX) -mfloat-abi=hard
armhf_slibdir = /lib/arm-linux-gnueabihf
armhf_libdir = /usr/lib/arm-linux-gnueabihf
# To be coinstallable with armel, we install ld.so to a multiarch directory
# from the beginning.
armhf_rtlddir = /lib/$(DEB_HOST_MULTIARCH)

define libc6-dev-armhf_extra_pkg_install
mkdir -p debian/libc6-dev-armhf/usr/include
cp -a debian/tmp-armhf/usr/include/bits \
	debian/libc6-dev-armhf/usr/include/
cp -a debian/tmp-armhf/usr/include/gnu \
	debian/libc6-dev-armhf/usr/include/
cp -a debian/tmp-armhf/usr/include/sys \
	debian/libc6-dev-armhf/usr/include/
cp debian/tmp-armhf/usr/include/fpu_control.h \
	debian/libc6-dev-armhf/usr/include/
endef
