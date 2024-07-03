AC_INIT([difx], [2.8.1], [])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC
AC_PROG_AWK
#AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 ipp.pc
])

dnl Set DIFXBITS, assuming POSIX environment (CHAR_BIT=8)
AC_CHECK_SIZEOF([size_t])
AC_MSG_CHECKING([what DIFXBITS should be])
DIFXBITS=$(( 8 * $ac_cv_sizeof_size_t ))
AC_MSG_RESULT([$DIFXBITS])
AC_DEFINE(DIFXBITS, $DIFXBITS)

AC_MSG_CHECKING([whether mk5daemon should be installed])
AC_ARG_ENABLE(mk5daemon,
  [AS_HELP_STRING([--enable-mk5daemon],
    [Install mk5daemon (not installed by default)])],
  [],
  [enable_mk5daemon=no])
AC_MSG_RESULT([$enable_mk5daemon])

AC_MSG_CHECKING([whether to build the difx_monitor])
AC_ARG_WITH(monitor,
  [AS_HELP_STRING([--with-monitor],
    [Build the difx_monitor (requires pgplot)])],
  [],
  [with_monitor=no])
AC_MSG_RESULT([$with_monitor])

AC_MSG_CHECKING([whether to build filterbank (requires pgplot)])
AC_ARG_WITH(fb,
  [AS_HELP_STRING([--with-fb],
    [Build the filterbank (requires pgplot)])],
  [],
  [with_fb=no])
AC_MSG_RESULT([$with_fb])

AC_MSG_CHECKING([whether to build guiserver])
AC_ARG_WITH(guiserver,
  [AS_HELP_STRING([--with-guiserver],
    [Build the guiserver])],
  [],
  [with_guiserver=no])
AC_MSG_RESULT([$with_guiserver])

AC_MSG_CHECKING([whether to build HOPS (which requires pgplot)])
AC_ARG_WITH(hops,
  [AS_HELP_STRING([--with-hops],
    [Build the HOPS])],
  [],
  [with_hops=no])
AC_MSG_RESULT([$with_hops])

AC_MSG_CHECKING([whether to build Mark6 support (which requires FUSE)])
AC_ARG_WITH(m6support,
  [AS_HELP_STRING([--with-m6support],
    [Build Mark6 support])],
  [],
  [with_m6support=no])
AC_MSG_RESULT([$with_m6support])

AC_MSG_CHECKING([whether to build Mark6 metadata support])
AC_ARG_WITH(mark6meta,
  [AS_HELP_STRING([--with-mark6meta],
    [Build Mark6 metadata support])],
  [],
  [with_mark6meta=no])
AC_MSG_RESULT([$with_mark6meta])

AC_MSG_CHECKING([whether to build PolConvert (requries CASA)])
AC_ARG_WITH(polconvert,
  [AS_HELP_STRING([--with-polconvert],
    [Build PolConvert])],
  [],
  [with_polconvert=no])
AC_MSG_RESULT([$with_polconvert])

AC_MSG_CHECKING([whether to build Python bindings (requries ctypes)])
AC_ARG_WITH(python,
  [AS_HELP_STRING([--with-python],
    [Build Python bindings])],
  [],
  [with_python=no])
AC_MSG_RESULT([$with_python])

AC_MSG_CHECKING([whether to build the datasim application (requires gsl)])
AC_ARG_WITH(datastream,
  [AS_HELP_STRING([--with-datastream],
    [Build the datasim application])],
  [],
  [with_datasim=yes])
AC_MSG_RESULT([$with_datasim])

AC_MSG_CHECKING([whether to build documentation])
AC_ARG_WITH(doc,
  [AS_HELP_STRING([--with-doc],
    [Build documentation])],
  [],
  [with_doc=no])
AC_MSG_RESULT([$with_doc])

AC_MSG_CHECKING([whether Intel Integrated Performance Primitives (IPP) should be enabled])
AC_ARG_WITH(ipp,
  [AS_HELP_STRING([--with-ipp], [Enable Intel Integrated Performance Primitives (IPP)])],
  [],
  [with_ipp=yes])
AC_MSG_RESULT($with_ipp)


dnl Check if IPP was enabled. Conditionally test for
dnl --with-ipp-includes and --with-ipp-libdir Set preprocessor and
dnl linker path accordingly.  Then process ipp.h for information to
dnl propagate pkg-config file ipp.pc

ipp_version_str=
ipp_version_major=
ipp_version_minor=
ipp_version_update=
ipp_interface_version_major=
ipp_interface_version_minor=
ipp_includes=
ipp_libdir=
ipp_defines=
ipp_libs="-lipps -lippvm -lippcore"
ipp_cflags=

AS_IF([test "x$with_ipp" != xno],
  [AC_MSG_CHECKING([whether ipp-includes was given])]
  [AC_ARG_WITH(ipp-includes,
    [AS_HELP_STRING([--with-ipp-includes=dir],
      [Include directory for IPP, typically at /opt/intel/ipp/include, /opt/intel/oneapi/ipp/latest/include, etc.])],
    [CPPFLAGS+=" -I${with_ipp_includes}" ipp_includes=$(readlink -f ${with_ipp_includes})],
    [with_ipp_includes=no])]
  [AC_MSG_RESULT($with_ipp_includes)]
  [AC_MSG_CHECKING([whether ipp-libdir was given])]
  [AC_ARG_WITH(ipp-libdir,
    [AS_HELP_STRING([--with-ipp-libdir=dir],
      [Directory where IPP libraries are found, typically at /opt/intel/ipp/lib, /opt/intel/oneapi/ipp/latest/lib, etc.])],
    [LDFLAGS+=" -L${with_ipp_libdir}" ipp_libdir=$(readlink -f ${with_ipp_libdir})
     AS_IF([test "x$ipp_libdir" != x], 
       [ipp_libs="-Wl,-rpath,$(readlink -f $with_ipp_libdir) -L$ipp_libdir $ipp_libs"],
       [])
    ],
    [with_ipp_libdir=no])]
  [AC_MSG_RESULT($with_ipp_libdir)]
  [AC_LANG_PUSH(C)]
  [AC_CHECK_HEADERS([ipp.h], [],
    [AC_MSG_ERROR([No IPP library includes found.])],[])]
  [
  dnl
  dnl Find IPP version information by compiling a test program.
  dnl This is safer than grep'ing files.
  AC_MSG_CHECKING([IPP version])
  AC_RUN_IFELSE([
   AC_LANG_PROGRAM(
   [[#include <stdio.h>]]
   [[#include "ipp.h"]]
   [[#define xstr(s) str(s)]]
   [[#define str(s) #s]]
   ,
   [[FILE *fp = fopen("conftest.out", "w");]]
   [[fputs(IPP_VERSION_STR"\n", fp);]]
   [[fputs(xstr(IPP_VERSION_MAJOR)"\n", fp);]]
   [[fputs(xstr(IPP_VERSION_MINOR)"\n", fp);]]
   [[fputs(xstr(IPP_VERSION_UPDATE)"\n", fp);]]      
   [[#ifdef IPP_INTERFACE_VERSION_MAJOR]]
   [[fputs(xstr(IPP_INTERFACE_VERSION_MAJOR)"\n", fp);]]
   [[#else]]
   [[fputs("\n", fp);]]
   [[#endif]]
   [[#ifdef IPP_INTERFACE_VERSION_MINOR]]
   [[fputs(xstr(IPP_INTERFACE_VERSION_MINOR)"\n", fp);]]
   [[#else]]
   [[fputs("\n", fp);]]
   [[#endif]]
   [[fclose(fp);]]
   )],
   [ipp_version_str=$($AWK 'FNR==1' conftest.out)
    ipp_version_major=$($AWK 'FNR==2' conftest.out)
    ipp_version_minor=$($AWK 'FNR==3' conftest.out)
    ipp_version_update=$($AWK 'FNR==4' conftest.out)     
    ipp_interface_version_major=$($AWK 'FNR==5' conftest.out)
    ipp_interface_version_minor=$($AWK 'FNR==6' conftest.out)
    AC_MSG_RESULT([$ipp_version_str,
                        major=$ipp_version_major, minor=$ipp_version_minor, update=$ipp_version_update
                        interface major=$ipp_interface_version_major, interface minor=$ipp_interface_version_minor])],
   [AC_MSG_ERROR([no value found.])]
  )
  AC_SEARCH_LIBS([ippInit],[ippcore], [], [])
  AC_LANG_POP(C)
  AS_IF([test $(($ipp_version_major)) -ge 9], [ipp_defines="-DIPP9"], [])
  ],
  [])

AS_IF([test "x$ipp_defines" != x],
  [ipp_cflags+=" $ipp_defines"],
  [])
AS_IF([test "x$ipp_includes" != x],
  [ipp_cflags+=" -I$ipp_includes"],
  [])
AC_SUBST([ipp_version_str])
AC_SUBST([ipp_version_major])
AC_SUBST([ipp_version_minor])
AC_SUBST([ipp_version_update])
AC_SUBST([ipp_interface_version_major])
AC_SUBST([ipp_interface_version_minor])
AC_SUBST([ipp_libdir])
AC_SUBST([ipp_includes])
AC_SUBST([ipp_defines])
AC_SUBST([ipp_cflags])
AC_SUBST([ipp_libs])


AC_OUTPUT
