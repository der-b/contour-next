# tests which are expected to fail
XFAIL_TEST =

# uncommend if hard failures should be threated as normal errors
# DISABLE_HARD_ERRORS = 1

AM_LDFLAGS = -pthread
AM_CFLAGS = -g -O0 -Wall -lcrypto
#SRC   = $(top_srcdir)/src
#HEAD  = $(top_srcdir)/include

# disable valgrind test for static code analyzer
LOG_COMPILER=
AM_LOG_FLAGS=

EXTRA_DIST=cppcheck.suppressions.list.txt cppcheck.sh

if COND_CPPCHECK
  TESTS = cppcheck.sh
endif

