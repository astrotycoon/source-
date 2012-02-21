## make file for liblinklist

MAINFILE=liblinklist.so
LIBVER=1.0.3
LIBFILE=$(MAINFILE).$(LIBVER)
SONAME=$(MAINFILE).3

MANPATH=/usr/local/man


all: $(LIBFILE)


LIBS=
ARGS=-g -Wall
REQH=



liblinklist.o: linklist.c linklist.h $(REQH)
	gcc -c -fPIC linklist.c -o $@ $(ARGS)

liblinklist.a: liblinklist.o
	@>$@
	@rm $@
	ar rcs $@ $^

$(LIBFILE): liblinklist.o
	gcc -shared -Wl,-soname,$(SONAME) -o $@ $^


lib-test: lib-test.c $(REQH) /usr/include/linklist.h
	gcc lib-test.c -o $@ -llinklist

install: $(LIBFILE) linklist.h 
	cp linklist.h /usr/include/
	cp $(LIBFILE) /usr/lib/
	@-test -e /usr/lib/$(MAINFILE) && rm /usr/lib/$(MAINFILE)
	ln -s /usr/lib/$(LIBFILE) /usr/lib/$(MAINFILE)
	nice ldconfig
	@echo "Install complete."

manpages: man/liblinklist.3 man/ll_free.3 man/ll_get_head.3 man/ll_get_tail.3 man/ll_init.3 man/ll_pop_head.3 man/ll_pop_tail.3 man/ll_push_head.3 man/ll_push_tail.3 man/ll_count.3 man/ll_join_str.3 man/ll_move_head.3 man/ll_move_tail.3
	@mkdir tmp.install
	@cp man/* tmp.install/
	@gzip tmp.install/*.3
	cp tmp.install/*.3.gz $(MANPATH)/man3/
	@rm -r tmp.install


clean:
	@-rm liblinklist.o
	@-rm liblinklist.so*

