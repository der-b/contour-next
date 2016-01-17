# tests which are expected to fail
#XFAIL_TEST =

# uncommend if hard failures should be threated as normal errors
# DISABLE_HARD_ERRORS = 1

#AM_LDFLAGS =
AM_CFLAGS = -g -O0 -Wall

FILES = 
FILES += unittest.c
FILES += ut_state.c
#FILES += ${top_srcdir}/src/utils/list/list_array.c
#FILES += ${top_srcdir}/src/utils/list/list_stlinked.c
#FILES += ${top_srcdir}/src/utils/list/list_dlinked.c
FILES += ../../../src/utils/list/list_array.c
FILES += ../../../src/utils/list/list_stlinked.c
FILES += ../../../src/utils/list/list_dlinked.c

noinst_HEADERS = unittest.h

#AM_CFLAGS = -Werror -Wall -I$(top_srcdir)/include

check_PROGRAMS = array array_ut_equal stlinked stlinked_ut_equal dlinked dlinked_ut_equal

TESTS = array stlinked dlinked stlinked_ut_equal dlinked_ut_equal array_ut_equal
array_SOURCES = array.c $(FILES)
stlinked_SOURCES = stlinked.c $(FILES)
dlinked_SOURCES = dlinked.c $(FILES)
stlinked_ut_equal_SOURCES = stlinked_ut_equal.c ut_equal.c $(FILES)
dlinked_ut_equal_SOURCES = dlinked_ut_equal.c ut_equal.c $(FILES)
array_ut_equal_SOURCES = array_ut_equal.c ut_equal.c $(FILES)

