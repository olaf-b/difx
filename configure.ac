dnl This is the top level autoconf file for DIFX Made to replace the
dnl use of shell-script and python script install-difx to have a more
dnl coherent build set-up that ties in with the already existing
dnl autoconf/automake toolset used by the separate tools contained
dnl herein.
dnl
dnl The original autoconf/automake setup is separate for each libary
dnl and application and pkg-config is used througout.  This requires
dnl setting a lot of variables when building using this top level
dnl configure.ac.  An example installations using auxillary homebrew
dnl packages on a Mac OS arm64 running Darwin requires a typical setup
dnl like this:
dnl
dnl    CPPFLAGS="-I/opt/homebrew/include \
dnl    -I$(pwd)/libraries/codifio/src \
dnl    -I$(pwd)/libraries/vdifio/src \
dnl    -I$(pwd)/libraries/mark5access \
dnl    -I$(pwd)/libraries/difxmessage"
dnl    LDFLAGS="-L$(pwd)/libraries/codifio/src \
dnl    -L$(pwd)/libraries/vdifio/src \
dnl    -L$(pwd)/libraries/difxmessage/difxmessage \
dnl    -L$(pwd)/libraries/mark5access/mark5access \
dnl    -L$(pwd)/libraries/difxmessage/mark5ipc \
dnl    -L$(pwd)/libraries/codifio/src \
dnl    -L$(pwd)/libraries/dirlist/src \
dnl    -L/opt/homebrew/lib"
dnl    LIBS="-lmpi"
dnl    PKG_CONFIG_PATH=$(pwd)/libraries/difxmessage
dnl    PKG_CONFIG_PATH+=:$(pwd)/libraries/vdifio
dnl    PKG_CONFIG_PATH+=:$(pwd)/libraries/mark5access
dnl    PKG_CONFIG_PATH+=:$(pwd)/libraries/difxio
dnl    PKG_CONFIG_PATH+=:$(pwd)/libraries/dirlist
dnl    
dnl    autoreconf -i --force -v && \
dnl        ./configure CPPFLAGS="$CPPFLAGS" LDFLAGS="$LDFLAGS" LIBS="$LIBS" \
dnl            PKG_CONFIG_PATH="$PKG_CONFIG_PATH" \
dnl            --enable-difx2profile=no --enable-misc=no --enable-vis2screen=no \
dnl            --enable-difx2fits=no --enable-vex2difx=no \
dnl            --with-ipp=no --enable-mpifxcorr --enable-misc=no && \
dnl                make
dnl
dnl Obviously, there is no need for using pkg-config on each sub
dnl project, and the software should be set up without pkg-config for
dnl the individual components.  In stead, the top level project should
dnl produce the pkg-config .pc files for an installation, and set
dnl library and build dependencies accordingly.  This change to /not/
dnl using sub-level PKG_CHECK_MODULES and AC_CONFIG_SUBDIRS is work in
dnl pending.  But compared to the alternative with sourcing
dnl shell-scipts and invoking a custom and non-standard python script
dnl install-difx, the use of an autotools build system incurs less
dnl surprises allowing the classical
dnl
dnl    ./configure && make && make install
dnl
dnl (with adjustments).  When a transition away from sub-project use
dnl of PKG_CHECK_MODULES and set-up of dependencies, it is expected
dnl that the system intrpospection and build will become as simple as
dnl the above.

AC_INIT([difx], [2.8.1], [])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_PROG_AWK
dnl AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 ipp.pc
])

AC_CHECK_SIZEOF([size_t])

AC_MSG_CHECKING([what DIFXBITS should be])
DIFXBITS=$(( 8 * $ac_cv_sizeof_size_t ))
AC_MSG_RESULT([$DIFXBITS])
AC_DEFINE(DIFXBITS, $DIFXBITS)

AC_MSG_CHECKING([wether to build perl support])
AC_ARG_ENABLE(perl,
  [AS_HELP_STRING([--enable-perl],
    [Add perl support (not enabled by default)])],
  [],
  [enable_perl=no])
AC_MSG_RESULT([$enable_perl])

AC_MSG_CHECKING([wether to build the difxio component])
AC_ARG_ENABLE(difxio,
  [AS_HELP_STRING([--enable-difxio],
    [Build difxio (enabled by default)])],
  [],
  [enable_difxio=yes])
AC_MSG_RESULT([$enable_difxio])

AC_MSG_CHECKING([wether to build the codifio component])
AC_ARG_ENABLE(codifio,
  [AS_HELP_STRING([--enable-codifio],
    [Build codifio (enabled by default)])],
  [],
  [enable_codifio=yes])
AC_MSG_RESULT([$enable_codifio])

AC_MSG_CHECKING([wether to build the difxmessage component])
AC_ARG_ENABLE(difxmessage,
  [AS_HELP_STRING([--enable-difxmessage],
    [Build difxmessage (enabled by default)])],
  [],
  [enable_difxmessage=yes])
AC_MSG_RESULT([$enable_difxmessage])

AC_MSG_CHECKING([wether to build the dirlist component])
AC_ARG_ENABLE(dirlist,
  [AS_HELP_STRING([--enable-dirlist],
    [Build dirlist (enabled by default)])],
  [],
  [enable_dirlist=yes])
AC_MSG_RESULT([$enable_dirlist])

AC_MSG_CHECKING([wether to build the mark5access component])
AC_ARG_ENABLE(mark5access,
  [AS_HELP_STRING([--enable-mark5access],
    [Build mark5access (enabled by default)])],
  [],
  [enable_mark5access=yes])
AC_MSG_RESULT([$enable_ENABLE])

AC_MSG_CHECKING([wether to build the mark6meta component])
AC_ARG_ENABLE(mark6meta,
  [AS_HELP_STRING([--enable-mark6meta],
    [Build mark6meta (not enabled by default)])],
  [],
  [enable_mark6meta=no])
AC_MSG_RESULT([$enable_mark5meta])

AC_MSG_CHECKING([wether to build the vdifio component])
AC_ARG_ENABLE(vdifio,
  [AS_HELP_STRING([--enable-vdifio],
    [Build vdifio (enabled by default)])],
  [],
  [enable_vdifio=yes])
AC_MSG_RESULT([$enable_vdifio])

AC_MSG_CHECKING([wether to build the calcif2 component])
AC_ARG_ENABLE(calcif2,
  [AS_HELP_STRING([--enable-calcif2],
    [Build calcif2 (not enabled by default)])],
  [],
  [enable_calcif2=no])
AC_MSG_RESULT([$enable_calcif2])

AC_MSG_CHECKING([wether to build the difx2profile component])
AC_ARG_ENABLE(difx2profile,
  [AS_HELP_STRING([--enable-difx2profile],
    [Build difx2profile (enabled by default)])],
  [],
  [enable_difx2profile=yes])
AC_MSG_RESULT([$enable_difx2profile])

AC_MSG_CHECKING([wether to build the misc component])
AC_ARG_ENABLE(misc,
  [AS_HELP_STRING([--enable-misc],
    [Build misc (enabled by default)])],
  [],
  [enable_misc=yes])
AC_MSG_RESULT([$enable_misc])

AC_MSG_CHECKING([wether to build the vis2screen component])
AC_ARG_ENABLE(vis2screen,
  [AS_HELP_STRING([--enable-vis2screen],
    [Build vis2screen (enabled by default)])],
  [],
  [enable_vis2screen=yes])
AC_MSG_RESULT([$enable_vis2screen])

AC_MSG_CHECKING([wether to build the calcserver component])
AC_ARG_ENABLE(calcserver,
  [AS_HELP_STRING([--enable-calcserver],
    [Build calcserver (not enabled by default)])],
  [],
  [enable_calcserver=no])
AC_MSG_RESULT([$enable_calcserver])

AC_MSG_CHECKING([wether to build the difx2fits component])
AC_ARG_ENABLE(difx2fits,
  [AS_HELP_STRING([--enable-difx2fits],
    [Build difx2fits (enabled by default)])],
  [],
  [enable_difx2fits=yes])
AC_MSG_RESULT([$enable_difx2fits])

AC_MSG_CHECKING([wether to build the vex2difx component])
AC_ARG_ENABLE(vex2difx,
  [AS_HELP_STRING([--enable-vex2difx],
    [Build vex2difx (enabled by default)])],
  [],
  [enable_vex2difx=yes])
AC_MSG_RESULT([$enable_vex2difx])

AC_MSG_CHECKING([wether to build the difx2mark4 component])
AC_ARG_ENABLE(difx2mark4,
  [AS_HELP_STRING([--enable-difx2mark4],
    [Install difx2mark4 (not enabled by default)])],
  [],
  [enable_difx2mark4=no])
AC_MSG_RESULT([$enable_difx2mark4])

AC_MSG_CHECKING([wether to build the mpifxcorr component])
AC_ARG_ENABLE(mpifxcorr,
  [AS_HELP_STRING([--enable-mpifxcorr],
    [Install mpifxcorr (enabled by default)])],
  [],
  [enable_mpifxcorr=yes])
AC_MSG_RESULT([$enable_mpifxcorr])

AC_MSG_CHECKING([wether to build the mark6sg component])
AC_ARG_ENABLE(mark6sg,
  [AS_HELP_STRING([--enable-mark6sg],
    [Install mark6sg (enabled by default)])],
  [],
  [enable_mark6sg=no])
AC_MSG_RESULT([$enable_mark6sg])

AC_MSG_CHECKING([wether to build the difxfilterbank component])
AC_ARG_ENABLE(difxfilterbank,
  [AS_HELP_STRING([--enable-difxfilterbank],
    [Install difxfilterbank (not enabled by default)])],
  [],
  [enable_difxfilterbank=no])
AC_MSG_RESULT([$enable_difxfilterbank])

AC_MSG_CHECKING([wether to build the hops component])
AC_ARG_ENABLE(hops,
  [AS_HELP_STRING([--enable-hops],
    [Install hops (not enabled by default)])],
  [],
  [enable_hops=no])
AC_MSG_RESULT([$enable_hops])

AC_MSG_CHECKING([wether to build the m6support component])
AC_ARG_ENABLE(m6support,
  [AS_HELP_STRING([--enable-m6support],
    [Install m6support (not enabled by default)])],
  [],
  [enable_m6support=no])
AC_MSG_RESULT([$enable_m6support])

AC_MSG_CHECKING([wether to build the polconvert component])
AC_ARG_ENABLE(polconvert,
  [AS_HELP_STRING([--enable-polconvert],
    [Install polconvert (not enabled by default)])],
  [],
  [enable_polconvert=no])
AC_MSG_RESULT([$enable_polconvert])

AC_MSG_CHECKING([wether to build the guiServer component])
AC_ARG_ENABLE(guiServer,
  [AS_HELP_STRING([--enable-guiServer],
    [Install guiServer (not enabled by default)])],
  [],
  [enable_guiServer=no])
AC_MSG_RESULT([$enable_guiServer])

AC_MSG_CHECKING([wether to build the mk5daemon component])
AC_ARG_ENABLE(mk5daemon,
  [AS_HELP_STRING([--enable-mk5daemon],
    [Install mk5daemon (not enabled by default)])],
  [],
  [enable_mk5daemon=no])
AC_MSG_RESULT([$enable_mk5daemon])

AC_MSG_CHECKING([wether to build the difx_monitor component])
AC_ARG_ENABLE(difx_monitor,
  [AS_HELP_STRING([--enable-difx_monitor],
    [Install difx_monitor (not enabled by default)])],
  [],
  [enable_difx_monitor=no])
AC_MSG_RESULT([$enable_difx_monitor])

AC_MSG_CHECKING([wether to build the datasim component])
AC_ARG_ENABLE(datasim,
  [AS_HELP_STRING([--enable-datasim],
    [Install datasim (not enabled by default)])],
  [],
  [enable_datasim=no])
AC_MSG_RESULT([$enable_datasim])

AC_MSG_CHECKING([wether to build the python component])
AC_ARG_ENABLE(python,
  [AS_HELP_STRING([--enable-python],
    [Install python support (enabled by default)])],
  [],
  [enable_python=yes])
AC_MSG_RESULT([$enable_python])

AC_MSG_CHECKING([wether to build the espresso component])
AC_ARG_ENABLE(espresso,
  [AS_HELP_STRING([--enable-espresso],
    [Install espresso component (enabled by default)])],
  [],
  [enable_espresso=yes])
AC_MSG_RESULT([$enable_espresso])

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





## Look for ipp.h and determine version if IPP is enabled
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
  [AC_MSG_CHECKING([whether ipp-includes were given])]
  [AC_ARG_WITH(ipp-includes,
    [AS_HELP_STRING([--with-ipp-includes],
      [Include directory for IPP, typically at /opt/intel/ipp/include, /opt/intel/oneapi/ipp/latest/include, etc.])],
    [CPPFLAGS+=" -I${with_ipp_includes}" ipp_includes=$(readlink -f ${with_ipp_includes})],
    [with_ipp_includes=no])]
  [AC_MSG_RESULT($with_ipp_includes)]
  [AC_MSG_CHECKING([whether ipp-libdir was given])]
  [AC_ARG_WITH(ipp-libdir,
    [AS_HELP_STRING([--with-ipp-libdir],
      [Directory where IPP libraries are found, typically at /opt/intel/ipp/lib, /opt/intel/oneapi/ipp/latest/lib, etc.])],
    [LDFLAGS+=" -L${with_ipp_libdir}" ipp_libdir=$(readlink -f ${with_ipp_libdir})
    ipp_libs="-Wl,-rpath,$ipp_libdir -L$ipp_libdir $ipp_libs"],
    [with_ipp_libdir=no])]
  [AC_MSG_RESULT($with_ipp_libdir)]
  [AC_LANG_PUSH(C)]
  [AC_CHECK_HEADERS([ipp.h], [],
    [AC_MSG_ERROR([No IPP library includes found.])],[])]
  [dnl  
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

AS_IF([test "x$with_ipp" != xno],
  [ipp_cflags+=" $ipp_defines -I$ipp_includes"]
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

dnl libs
#AS_IF([test "x$enable_difxio" == xyes], [AX_SUBDIRS_CONFIGURE([libraries/difxio])])
AS_IF([test "x$enable_difxio" == xyes], [AC_CONFIG_SUBDIRS([libraries/difxio])])
AS_IF([test "x$enable_codifio" == xyes], [AC_CONFIG_SUBDIRS([libraries/codifio])])
AS_IF([test "x$enable_difxmessage" == xyes], [AC_CONFIG_SUBDIRS([libraries/difxmessage])])
AS_IF([test "x$enable_dirlist" == xyes], [AC_CONFIG_SUBDIRS([libraries/dirlist])])
AS_IF([test "x$enable_mark6sg" == xyes], [AC_CONFIG_SUBDIRS([libraries/mark6sg])])
AS_IF([test "x$enable_mark6meta" == xyes], [AC_CONFIG_SUBDIRS([libraries/mark6meta])])
AS_IF([test "x$enable_mark5access" == xyes], [AC_CONFIG_SUBDIRS([libraries/mark5access])])
AS_IF([test "x$enable_python" == xyes], [AC_CONFIG_SUBDIRS([libraries/python])])
AS_IF([test "x$enable_vdifio" == xyes], [AC_CONFIG_SUBDIRS([libraries/vdifio])])

AS_IF([test "x$enable_mpifxcorr" == xyes], [AC_CONFIG_SUBDIRS([mpifxcorr])])

dnl applications
AS_IF([test "x$enable_calcserver" == xyes], [AC_CONFIG_SUBDIRS([applications/calcserver])])
AS_IF([test "x$enable_difxcalc11" == xyes], [AC_CONFIG_SUBDIRS([applications/difxcalc11])])
AS_IF([test "x$enable_difx2fits" == xyes],  [AC_CONFIG_SUBDIRS([applications/difx2fits])])
AS_IF([test "x$enable_vex2difx" == xyes], [AC_CONFIG_SUBDIRS([applications/vex2difx])])
AS_IF([test "x$enable_difx2mark4" == xyes], [AC_CONFIG_SUBDIRS([applications/difx2mark4])])
AS_IF([test "x$enable_difxfilterbank" == xyes], [AC_CONFIG_SUBDIRS([applications/difxfilterbank])])
AS_IF([test "x$enable_hops" == xyes], [AC_CONFIG_SUBDIRS([applications/hops])])
AS_IF([test "x$enable_m6support" == xyes], [AC_CONFIG_SUBDIRS([applications/m6support])])
AS_IF([test "x$enable_polconvert" == xyes], [AC_CONFIG_SUBDIRS([applications/polconvert])])
AS_IF([test "x$enable_guiServer" == xyes], [AC_CONFIG_SUBDIRS([applications/guiServer])])
AS_IF([test "x$enable_mk5daemon" == xyes], [AC_CONFIG_SUBDIRS([applications/mk5daemon])])
AS_IF([test "x$enable_difx_monitor" == xyes], [AC_CONFIG_SUBDIRS([applications/difx_monitor])])
AS_IF([test "x$enable_datasim" == xyes], [AC_CONFIG_SUBDIRS([applications/datasim])])
AS_IF([test "x$enable_espresso" == xyes], [AC_CONFIG_SUBDIRS([applications/espresso])])

dnl utils
AS_IF([test "x$enable_calcif2" == xyes], [AC_CONFIG_SUBDIRS([utilities/calcif2])])
AS_IF([test "x$enable_difx2profile" == xyes], [AC_CONFIG_SUBDIRS([utilities/pulsar/difx2profile])])
AS_IF([test "x$enable_misc" == xyes], [AC_CONFIG_SUBDIRS([utilities/misc])])
AS_IF([test "x$enable_vis2screen" == xyes], [AC_CONFIG_SUBDIRS([utilities/vis2screen])])


AC_SUBST([subdirs])

AC_OUTPUT
