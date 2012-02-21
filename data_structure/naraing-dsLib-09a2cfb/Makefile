# Makefile - top level makefile
# 

CC = gcc
CFLAGS = -c -g -O2 -Wall -Wno-unused-function -I./
RANLIB = ranlib
#DEFS = -DHAVE_CONFIG_H
DEFS = 
STATIC = 
LDFLAGS =  $(STATIC)
TARGET = libds.a
ARCH	:= $(shell uname -m)
OUTDIR	:= $(ARCH)
TC_OUT  = tc

DS_OBJS = ll.o ds.o graph.o heap.o queue.o graph_ops.o sort.o utils.o
TC_OBJS = tc_ll.o tc.o tc_graph.o tc_heap.o tc_sort.o
ALGO_OBJS = sort.o

all: $(TARGET) $(TC_OUT)

$(TARGET): $(DS_OBJS) $(ALGO_OBJS)
	@echo Building dslib
	rm -f $@
	$(AR) cr $@ $(DS_OBJS) $(ALGO_OBJS)
	$(RANLIB) $@

$(TC_OUT): $(TARGET) $(TC_OBJS)
	@echo Building test case exec
	rm -f $@
	$(CC) -g -o $@ $(TC_OBJS) -lds -lrt -L. -I.

clean:
	rm -f $(DS_OBJS) $(ALGO_OBJS) $(TARGET) $(TC_OUT) $(TC_OBJS)
