PREFIX ?= /usr/local

VERSION = 0.1

all:
	@echo "Making shared library"
	@gcc -c -fPIC clist.c
	@gcc -shared -Wl,-soname,libclist.so.0 -o libclist.so.0.0 clist.o
	@echo "Making static library"
	@gcc -c -o clist_static.o clist.c
	@ar rcs libclist.a clist_static.o

install: all
	@echo "Installing libraries and headers"
	@mkdir -p "$(PREFIX)/lib"
	@mkdir -p "$(PREFIX)/include"
	@cp libclist.a "$(PREFIX)/lib"
	@cp libclist.so.0.0 "$(PREFIX)/lib"
	@/sbin/ldconfig -n "$(PREFIX)/lib"
	@ln "$(PREFIX)/lib/libclist.so.0" "$(PREFIX)/lib/libclist.so"
	@cp clist.h "$(PREFIX)/include"

clean:
	@echo -n $(shell rm -f *.o libclist* test)

test: all
	@gcc -o test test.c -L. -lclist

dist: clean
	@echo "Creating package"
	@mkdir -p "clist-$(VERSION)"
	@cp clist.c clist.h test.c Makefile README "clist-$(VERSION)"
	@tar -cf "clist-$(VERSION).tar" "clist-$(VERSION)"
	@gzip "clist-$(VERSION).tar"
	@rm -Rf "clist-$(VERSION)"

.PHONY: all install clean test dist
