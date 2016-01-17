# tests which are expected to fail
XFAIL_TEST =

# uncommend if hard failures should be threated as normal errors
# DISABLE_HARD_ERRORS = 1

AM_LDFLAGS =
AM_CFLAGS = -g -O0 -Wall -I$(top_srcdir)/include

noinst_HEADERS = test_header.h

check_PROGRAMS = test_base64

TESTS = test_base64

test_base64_SOURCES = test_base64.c ../../../src/utils/base64/base64.c

