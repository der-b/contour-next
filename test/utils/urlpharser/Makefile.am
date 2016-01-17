# tests which are expected to fail
XFAIL_TEST =

# uncommend if hard failures should be threated as normal errors
# DISABLE_HARD_ERRORS = 1

AM_LDFLAGS =
AM_CFLAGS = -g -O0 -Wall -I$(top_srcdir)/include

noinst_HEADERS = test_header.h

check_PROGRAMS = test_pharse_url

TESTS = test_pharse_url

test_pharse_url_SOURCES = test_pharse_url.c ../../../src/utils/urlpharser/urlpharser.c

