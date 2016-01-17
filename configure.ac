AC_INIT([contour-next],[0.0],[der-b@der-b.com])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])
AC_PROG_CC
AM_PROG_CC_C_O

AC_ARG_ENABLE([valgrind],
              [AS_HELP_STRING([--disable-valgrind],[Disables the usage of valgrind for test cases.])],
	      [],
	      [enable_valgrind=yes])

if test $enable_valgrind = yes; then
  AC_CHECK_PROG(HAVE_PROG_VALGRIND, [valgrind], [yes], [no])
  if test "$HAVE_PROG_VALGRIND" != yes; then 
    AC_MSG_WARN([Valgrind is not installed. It is recommented to install Valgrind when 'make check' is used.])
  else
    LOG_COMPILER=valgrind  
    AM_LOG_FLAGS="--leak-check=yes --error-exitcode=1 --quiet"
    AC_SUBST(LOG_COMPILER)
    AC_SUBST(AM_LOG_FLAGS)
  fi
else
  HAVE_PROG_VALGRIND=no
fi

AC_ARG_ENABLE([cppcheck],
              [AS_HELP_STRING([--disable-cppcheck],[Disables the usage of cppcheck for static code analyzis.])],
	      [],
	      [enable_cppcheck=yes])

if test $enable_cppcheck = yes; then
  AC_CHECK_PROG(HAVE_PROG_CPPCHECK, [cppcheck], [yes], [no])
else
  HAVE_PROG_CPPCHECK=no
fi
AM_CONDITIONAL([COND_CPPCHECK], [test "$HAVE_PROG_CPPCHECK" = yes])

# AC_DEFINE([CONF_DEFAULT_NUMBER_OF_DEVICES], [0], ["Number of devices connected to the controller"])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/astm/Makefile
  src/contour-next-usb/Makefile
  src/simple_protocol/Makefile
  src/utils/Makefile
  src/utils/list/Makefile
  src/utils/log/Makefile
  src/utils/urlpharser/Makefile
  src/utils/base64/Makefile
  include/Makefile
  include/astm/Makefile
  include/contour-next-usb/Makefile
  include/simple_protocol/Makefile
  include/utils/Makefile
  include/utils/list/Makefile
  include/utils/log/Makefile
  test/Makefile
  test/utils/Makefile
  test/utils/list/Makefile
  test/utils/urlpharser/Makefile
  test/utils/base64/Makefile
  test/staticAnalyzing/Makefile
])
AC_OUTPUT
