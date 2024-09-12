AC_PREREQ(2.50)
AC_INIT([DIFX2PROFILE], [0.6], [difxusers at googlegroups.com])
PACKAGE_VERSION=AC_PACKAGE_VERSION
AC_SUBST([PACKAGE_VERSION])
AC_SUBST([LIBRARY_VERSION])

AM_INIT_AUTOMAKE

AC_LANG([C++])
AC_CONFIG_SRCDIR([src/difx2profile.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

dnl **********************************************************

# Checks for libraries.
PKG_CHECK_MODULES(DIFXIO, difxio >= 3.7.0)
PKG_CHECK_MODULES(FXCORR, fxcorr >= 2.8)
PKG_CHECK_MODULES(DIFXMESSAGE, difxmessage >= 2.8.0)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

CXXFLAGS="${CXXFLAGS} ${DIFXIO_CFLAGS} ${DIFXMESSAGE_CFLAGS} ${FXCORR_CFLAGS}"
LIBS=" ${FXCORR_LIBS} ${DIFXIO_LIBS} ${DIFXMESSAGE_LIBS} -lpthread"

AC_CONFIG_FILES([Makefile src/Makefile scripts/Makefile])
AC_OUTPUT()
