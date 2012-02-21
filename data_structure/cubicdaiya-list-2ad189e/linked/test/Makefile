
VPATH = ../

CC = gcc
CFLAGS = -Wall -g -std=c99
LFLAGS = -lcunit
OBJS = ltest.o list.o
TARGETS = ltest

.c.o:
	$(CC) $(CFLAGS) -c $<

all : $(TARGETS)

ltest : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS)

check :
	./ltest

clean :
	rm $(OBJS) $(TARGETS)

ltest.o: ltest.c ../list.h
list.o: ../list.c ../list.h
