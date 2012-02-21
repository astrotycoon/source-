LIB_SRCS  := lib/ll.c
LIB_OBJS  := $(LIB_SRCS:.c=.o)
LIB_HDRS  := lib/ll.h
LIB_CFLAGS := -O3

TEST_SRCS := test/test.c
TEST_OBJS := $(TEST_SRCS:.c=.o)
TEST_HDRS := lib/ll.h

CFLAGS := -Ilib/ -ansi -pedantic

all: Makefile ll.a runtest

$(LIB_OBJS): $(LIB_SRCS)
	gcc $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

$(TEST_OBJS): $(TEST_SRCS)
	gcc $(CFLAGS) $(TEST_CFLAGS) -c $< -o $@

ll.a: $(LIB_OBJS)
	$(AR) rcs ll.a $(LIB_OBJS)

runtest: $(TEST_OBJS) ll.a
	$(CC) -o $@ $(TEST_OBJS) ll.a -g

clean:
	-rm $(LIB_OBJS) ll.a $(TEST_OBJS) runtest
