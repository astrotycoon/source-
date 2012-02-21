#ifndef __UNITTEST_H__
#define __UNITTEST_H__

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int test_failed;

#define expect(cond)	do {						\
		if (!(cond)) {						\
			test_failed = 1;				\
			fprintf(stderr, __FILE__ ":%d: expectation `" #cond \
				"' failed.\n", __LINE__);		\
		}							\
	} while (0)

#define TEST(name)	do {					\
		int i, n;					\
		printf("test_" #name ": %n", &n);		\
		for (i = 0; i < 71 - n; ++i) {  printf(" "); }	\
		fflush(stdout);					\
		test_failed = 0;				\
		setup_testenv();				\
		test_ ## name ();				\
		if (test_failed) {				\
			printf("[ \033[01;31mFAIL\033[0m ]\n");	\
		} else {					\
			printf("[ \033[01;32mPASS\033[0m ]\n");	\
		}						\
		teardown_testenv();				\
	} while (0)

struct sigaction old_sigaction_hup;
struct sigaction old_sigaction_int;
struct sigaction old_sigaction_quit;
struct sigaction old_sigaction_kill;
struct sigaction old_sigaction_term;

struct sigaction old_sigaction_ill;
struct sigaction old_sigaction_abrt;
struct sigaction old_sigaction_fpe;
struct sigaction old_sigaction_segv;
struct sigaction old_sigaction_pipe;
struct sigaction old_sigaction_bus;

static int oldstderr, newstderr;
static char tempfilename[21];
static int needs_teardown;

void teardown_testenv(void);

void int_handler(int signum) {
	if (write(oldstderr, "\n", 1) != 1) {
		exit(EXIT_FAILURE);
	}
	teardown_testenv();

	switch (signum) {
	case SIGHUP:
		if (old_sigaction_hup.sa_handler) {
			old_sigaction_hup.sa_handler(signum);
		} break;
	case SIGINT:
		if (old_sigaction_int.sa_handler) {
			old_sigaction_int.sa_handler(signum);
		} break;
	case SIGQUIT:
		if (old_sigaction_quit.sa_handler) {
			old_sigaction_quit.sa_handler(signum);
		} break;
	case SIGKILL:
		if (old_sigaction_kill.sa_handler) {
			old_sigaction_kill.sa_handler(signum);
		} break;
	case SIGTERM:
		if (old_sigaction_term.sa_handler) {
			old_sigaction_term.sa_handler(signum);
		} break;
	default:
		break;
	}

	exit(signum);
}

void fail_handler(int signum) {
	if (write(oldstderr, "[ \033[01;31mFAIL\033[0m ]\n", 21) != 21) {
		exit(EXIT_FAILURE);
	}
	teardown_testenv();

	switch (signum) {
	case SIGILL:
		if (old_sigaction_ill.sa_handler) {
			old_sigaction_ill.sa_handler(signum);
		} break;
	case SIGABRT:
		if (old_sigaction_abrt.sa_handler) {
			old_sigaction_abrt.sa_handler(signum);
		} break;
	case SIGFPE:
		if (old_sigaction_fpe.sa_handler) {
			old_sigaction_fpe.sa_handler(signum);
		} break;
	case SIGSEGV:
		if (old_sigaction_segv.sa_handler) {
			old_sigaction_segv.sa_handler(signum);
		} break;
	case SIGPIPE:
		if (old_sigaction_pipe.sa_handler) {
			old_sigaction_pipe.sa_handler(signum);
		} break;
	case SIGBUS:
		if (old_sigaction_bus.sa_handler) {
			old_sigaction_bus.sa_handler(signum);
		} break;
	default:
		break;
	}

	exit(signum);
}

static struct sigaction sig_testfail = {
	{ fail_handler },
	{{ 0 }},
	0,
	NULL
};
static struct sigaction sig_testint = {
	{ int_handler },
	{{ 0 }},
	0,
	NULL
};

void setup_testenv(void) {
	oldstderr = dup(STDERR_FILENO);
	if (oldstderr < 0) {
		exit(EXIT_FAILURE);
	}
	strncpy(tempfilename, "/tmp/stderr.XXXXXXXX", 21);
	newstderr = mkstemp(tempfilename);
	if (newstderr < 0) {
		exit(EXIT_FAILURE);
	}
	if (dup2(newstderr, STDERR_FILENO) != STDERR_FILENO) {
		exit(EXIT_FAILURE);
	}

	sigaction(SIGHUP, &sig_testint, &old_sigaction_hup);
	sigaction(SIGINT, &sig_testint, &old_sigaction_int);
	sigaction(SIGQUIT, &sig_testint, &old_sigaction_quit);
	sigaction(SIGKILL, &sig_testint, &old_sigaction_kill);
	sigaction(SIGTERM, &sig_testint, &old_sigaction_term);

	sigaction(SIGILL, &sig_testfail, &old_sigaction_ill);
	sigaction(SIGABRT, &sig_testfail, &old_sigaction_abrt);
	sigaction(SIGFPE, &sig_testfail, &old_sigaction_fpe);
	sigaction(SIGSEGV, &sig_testfail, &old_sigaction_segv);
	sigaction(SIGPIPE, &sig_testfail, &old_sigaction_pipe);
	sigaction(SIGBUS, &sig_testfail, &old_sigaction_bus);

	needs_teardown = 1;
}

void teardown_testenv(void) {
	char buf[256];
	ssize_t ret;
	if (needs_teardown) {
		lseek(newstderr, 0, SEEK_SET);
		while ((ret = read(newstderr, buf, 256))) {
			if (write(oldstderr, buf, ret) != ret) {
				exit(EXIT_FAILURE);
			}
		}
		if (dup2(oldstderr, STDERR_FILENO) != STDERR_FILENO) {
			exit(EXIT_FAILURE);
		}
		close(newstderr);
		unlink(tempfilename);
		needs_teardown = 0;
	}

	sigaction(SIGHUP, &old_sigaction_hup, &sig_testint);
	sigaction(SIGINT, &old_sigaction_int, &sig_testint);
	sigaction(SIGQUIT, &old_sigaction_quit, &sig_testint);
	sigaction(SIGKILL, &old_sigaction_kill, &sig_testint);
	sigaction(SIGTERM, &old_sigaction_term, &sig_testint);

	sigaction(SIGILL, &old_sigaction_ill, &sig_testfail);
	sigaction(SIGABRT, &old_sigaction_abrt, &sig_testfail);
	sigaction(SIGFPE, &old_sigaction_fpe, &sig_testfail);
	sigaction(SIGSEGV, &old_sigaction_segv, &sig_testfail);
	sigaction(SIGPIPE, &old_sigaction_pipe, &sig_testfail);
	sigaction(SIGBUS, &old_sigaction_bus, &sig_testfail);
}

#endif  /* __UNITTEST_H__ */
