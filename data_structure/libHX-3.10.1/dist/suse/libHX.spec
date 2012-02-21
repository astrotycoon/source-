
Name:		libHX
%define lname	libHX27
Version:	3.10.1
Release:	jng<CI_CNT>
Group:		System/Libraries
URL:		http://libhx.sf.net/
Summary:	Useful collection of routines for C and C++ programming
License:	LGPLv2+
Source:		http://downloads.sf.net/libhx/libHX-%version.tar.xz
Source3:	http://downloads.sf.net/libhx/libHX-%version.tar.xz.asc
Source9:	baselibs.conf
BuildRoot:	%_tmppath/%name-%version-build
BuildRequires:	gcc-c++, pkg-config, xz
# no, libxml2-devel is NOT required because nothing
# that requires it is going to be compiled.
# gcc-c++ is pretty optional and only used for make check 

%define debug_package_requires %lname = %version-%release

%description
libHX is a C library (with some C++ bindings available) that provides
data structures and functions commonly needed, such as maps, deques,
linked lists, string formatting and autoresizing, option and config
file parsing, type checking casts and more.

libHX aids in quickly writing up C and C++ data processing programs,
by consolidating tasks that often happen to be open-coded, such as
(simple) config file reading, option parsing, directory traversal,
and others, into a library. The focus is on reducing the amount of
time (and secondarily, the amount of code) a developer has to spend
for otherwise implementing such.


Author(s):
----------
	Jan Engelhardt

%package -n %lname
Group:		System/Libraries
Summary:	Useful collection of routines for C and C++ programming

%description -n %lname
libHX is a C library (with some C++ bindings available) that provides
data structures and functions commonly needed, such as maps, deques,
linked lists, string formatting and autoresizing, option and config
file parsing, type checking casts and more.

libHX aids in quickly writing up C and C++ data processing programs,
by consolidating tasks that often happen to be open-coded, such as
(simple) config file reading, option parsing, directory traversal,
and others, into a library. The focus is on reducing the amount of
time (and secondarily, the amount of code) a developer has to spend
for otherwise implementing such.


Author(s):
----------
	Jan Engelhardt

%package devel
Group:		Development/Libraries/C and C++
Summary:	Development files for libHX
Requires:	%lname = %version
%if "%{?vendor_uuid}" != ""
Provides:	%name-devel(vendor:%vendor_uuid) = %version-%release
%endif

%description devel
libHX is a C library (with some C++ bindings available) that provides
data structures and functions commonly needed, such as maps, deques,
linked lists, string formatting and autoresizing, option and config
file parsing, type checking casts and more.

libHX aids in quickly writing up C and C++ data processing programs,
by consolidating tasks that often happen to be open-coded, such as
(simple) config file reading, option parsing, directory traversal,
and others, into a library. The focus is on reducing the amount of
time (and secondarily, the amount of code) a developer has to spend
for otherwise implementing such.


Author(s):
----------
	Jan Engelhardt

%prep
%setup -q

%build
if [ ! -e configure ]; then
	./autogen.sh;
fi;
%configure
make %{?_smp_mflags};

%install
b="%buildroot";
rm -Rf "$b";
mkdir "$b";
make install DESTDIR="$b" docdir="%_docdir/%name";
find "$b/%_libdir" -type f -name "*.la" -delete;
mkdir -p "$b/%_docdir/%name";
install -pm0644 doc/*.txt "$b/%_docdir/%name/";

%check
make check

%post -n %lname -p /sbin/ldconfig

%postun -n %lname -p /sbin/ldconfig

%files -n %lname
%defattr(-,root,root)
%_libdir/%{name}*.so.*

%files devel
%defattr(-,root,root)
%_libdir/%{name}*.so
%_libdir/pkgconfig/*
%_includedir/*
%_mandir/*/*
%docdir %_docdir/%name
%_docdir/%name

%changelog
