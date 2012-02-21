CC=gcc
CFLAGS+=-Wall -Werror -Wextra -g3 -I.
LDFLAGS+=-g3 -lrt

_libcleif_OBJS=skiplist.o tree.o
_klist_test_OBJS=klist_test.o
_skiplist_test_OBJS=skiplist_test.o skiplist.o
_tree_test_OBJS=tree_test.o tree.o
_withstmt_test_OBJS=withstmt_test.o
_skiplist_perf_OBJS=skiplist_perf.o skiplist.o

_OBJS=$(_libcleif_OBJS) \
	$(_klist_test_OBJS) \
	$(_skiplist_test_OBJS) \
	$(_tree_test_OBJS) \
	$(_withstmt_test_OBJS) \
	$(_skiplist_perf_OBJS)
_DEPS=$(_OBJS:.o=.d)

TEST_BINS=klist_test skiplist_test tree_test withstmt_test
klist_test_OBJS=$(addprefix .objs/,$(_klist_test_OBJS))
skiplist_test_OBJS=$(addprefix .objs/,$(_skiplist_test_OBJS))
tree_test_OBJS=$(addprefix .objs/,$(_tree_test_OBJS))
withstmt_test_OBJS=$(addprefix .objs/,$(_withstmt_test_OBJS))
PERF_BINS=skiplist_perf
skiplist_perf_OBJS=$(addprefix .objs/,$(_skiplist_perf_OBJS))
LIBS=libcleif.a
libcleif_OBJS=$(addprefix .objs/,$(_libcleif_OBJS))
OBJS=$(addprefix .objs/,$(_OBJS))
DEPS=$(addprefix .deps/,$(_DEPS))

cmd_cc_o_c=$(CC) $(CFLAGS) -o $@ -c $<
quiet_cmd_cc_o_c = CC       $*.o
cmd_md_d_c=$(CC) $(CFLAGS) -o $@ -MT .objs/$(patsubst %.c,%.o,$<) -M $<
quiet_cmd_md_d_c = MD       $*.d
cmd_ld_bin_o=$(CC) $(LDFLAGS) -o $@ $^
quiet_cmd_ld_bin_o = LD       $@
cmd_ar_a_o=$(AR) cru $@ $^
quiet_cmd_ar_a_o = AR       $@
cmd_TAGS=etags $^
quiet_cmd_TAGS = BUILD    TAGS
cmd_tags=ctags $^
quiet_cmd_tags = BUILD    tags
cmd_cscope=cscope -b $^
quiet_cmd_cscope = BUILD    cscope

ifdef VERBOSE
	quiet=
	Q=
else
	quiet=quiet_
	Q=@
endif

.PHONY: all tests perfs check clean allclean libs
all: tags TAGS cscope.out tests perfs libs

check: $(addprefix run_,$(TEST_BINS))
tests: $(TEST_BINS)
perfs: $(PERF_BINS)
libs: $(LIBS)

klist_test: $(klist_test_OBJS)
	@echo "   $($(quiet)cmd_ld_bin_o)"; $(cmd_ld_bin_o)
run_klist_test: klist_test
	./$<

skiplist_test: $(skiplist_test_OBJS)
	@echo "   $($(quiet)cmd_ld_bin_o)"; $(cmd_ld_bin_o)
run_skiplist_test: skiplist_test
	./$<

tree_test: $(tree_test_OBJS)
	@echo "   $($(quiet)cmd_ld_bin_o)"; $(cmd_ld_bin_o)
run_tree_test: tree_test
	./$<

withstmt_test: $(withstmt_test_OBJS)
	@echo "   $($(quiet)cmd_ld_bin_o)"; $(cmd_ld_bin_o)
run_withstmt_test: withstmt_test
	./$<

libcleif.a: $(libcleif_OBJS)
	@echo "   $($(quiet)cmd_ar_a_o)"; $(cmd_ar_a_o)

skiplist_perf: $(skiplist_perf_OBJS)
	@echo "   $($(quiet)cmd_ld_bin_o)"; $(cmd_ld_bin_o)
run_skiplist_perf: skiplist_perf
	./$<

.objs/%.o: .deps/%.d
.deps/%.d: %.c
	$(Q)[[ -d .deps ]] || mkdir -p .deps
	@echo "   $($(quiet)cmd_md_d_c)"; $(cmd_md_d_c)

.objs/%.o: %.c
	$(Q)[[ -d .objs ]] || mkdir -p .objs
	@echo "   $($(quiet)cmd_cc_o_c)"; $(cmd_cc_o_c)

clean:
	$(Q)rm -f $(OBJS) $(TEST_BINS) $(PERF_BINS) $(LIBS)

allclean: clean
	$(Q)rm -f .*.sw? "#"*"#" *\~ tags TAGS cscope.out $(DEPS)

TAGS tags cscope: $(wildcard *.c) $(wildcard *.h)
TAGS:
	@echo "   $($(quiet)cmd_TAGS)"; $(cmd_TAGS)
tags:
	@echo "   $($(quiet)cmd_tags)"; $(cmd_tags)
cscope.out:
	@echo "   $($(quiet)cmd_cscope)"; $(cmd_cscope)

-include $(DEPS)
