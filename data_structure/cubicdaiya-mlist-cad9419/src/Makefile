
CC = gcc
AR = ar
CFLAGS = -Wall -g -std=c99 -fPIC
ARFLAGS = r
STATIC_LFILE = libmlist.a
DYNAMIC_LFILE = libmlist.so
OBJS = mlist.o

.c.o:
	$(CC) $(CFLAGS) -c $<

$(STATIC_LFILE): $(OBJS)
	$(AR) $(ARFLAGS) $(STATIC_LFILE) $(OBJS)
	$(CC) -shared $(CFLAGS) $(OBJS) -o $(DYNAMIC_LFILE)

clean:
	rm $(OBJS) $(STATIC_LFILE) $(DYNAMIC_LFILE)


mlist.o: mlist.c mlist.h

