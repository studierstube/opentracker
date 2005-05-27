############################################################################
# Usage:
#   STB_AC_HAVE_H323_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the OpenH323 development system.  If it is found,
#   the set of variables listed below are set up as described and made
#   available to the configure script.
#
# Autoconf Variables:
# < $stb_ac_h323_avail       true | false
# < $stb_ac_h323_cppflags    (extra flags the preprocessor needs)
# < $stb_ac_h323_ldflags     (extra flags the linker needs)
# < $stb_ac_h323_libs        (link library flags the linker needs)
# < $stb_ac_h323_rpath       (library directory if not in linker search path)
#
# Author:
#   Tamer Fahmy <tamer@tammura.at>
#
# TODO:
#

AC_DEFUN([STB_AC_HAVE_H323_IFELSE], [
AC_PREREQ([2.53])

# official variables
stb_ac_h323_cppflags=
stb_ac_h323_ldflags=
stb_ac_h323_libs=
stb_ac_h323_root=
stb_ac_h323_rpath=

# internal variables
stb_ac_h323_extrapath=

AC_ARG_WITH([h323],
  AC_HELP_STRING([--with-h323=DIR], [give prefix location of H323]),
  [stb_ac_h323_extrapath=$withval], [])
AC_ARG_WITH([h323],
  AC_HELP_STRING([--with-ptlib=DIR], [give prefix location of H323]),
  [stb_ac_ptlib_extrapath=$withval], [])


if test -z "$stb_ac_h323_extrapath"; then
  stb_ac_h323_extrapath="$ac_default_prefix"
  stb_ac_h323_cppflags="-I$ac_default_prefix/include/openh323"
  stb_ac_h323_ldflags=""
else
  case "$stb_ac_h323_extrapath" in
    [[\\/]]* | ?:[[\\/]]* ) : 
        stb_ac_h323_rpath="-R $stb_ac_h323_extrapath/lib" ;;
    * ) stb_ac_h323_extrapath=`pwd`/$stb_ac_h323_extrapath
        stb_ac_h323_rpath="-R $stb_ac_h323_extrapath/lib" ;;
  esac
  stb_ac_h323_cppflags="-I$stb_ac_h323_extrapath/include/openh323"
  stb_ac_h323_ldflags="-L$stb_ac_h323_extrapath/lib"
fi

if test -z "$stb_ac_ptlib_extrapath"; then
  stb_ac_ptlib_extrapath="$ac_default_prefix"
  stb_ac_ptlib_cppflags="-I$ac_default_prefix/include/ptlib/unix -I$ac_default_prefix/include/ptlib/ptclib"
  stb_ac_ptlib_ldflags=""
else
  case "$stb_ac_ptlib_extrapath" in
    [[\\/]]* | ?:[[\\/]]* ) : 
        stb_ac_ptlib_rpath="-R $stb_ac_ptlib_extrapath/lib" ;;
    * ) stb_ac_ptlib_extrapath=`pwd`/$stb_ac_ptlib_extrapath
        stb_ac_ptlib_rpath="-R $stb_ac_ptlib_extrapath/lib" ;;
  esac
  stb_ac_ptlib_cppflags="-I$stb_ac_ptlib_extrapath/include/ptlib/unix -I$stb_ac_ptlib_extrapath/include/ptclib"
  stb_ac_ptlib_ldflags="-L$stb_ac_ptlib_extrapath/lib"
fi

stb_ac_h323_root=$stb_ac_h323_extrapath

case $host_os in
  linux-gnu | linux | Linux)
    H323_CFLAGS="-DP_LINUX -D_REENTRANT $ARCH_H323_CFLAGS -DHAS_OSS -DNDEBUG -DPHAS_TEMPLATES -DP_USE_PRAGMA -DPTRACING"
    H323_LIBS="-lopenh323 -ldl -lpt -lpthread"
    HAS_SPEEX="enabled"
    ;;

  FreeBSD* | freebsd* )
    H323_CFLAGS="-DP_FREEBSD=400001 $ARCH_H323_CFLAGS -DPHAS_TEMPLATES -DP_USE_PRAGMA -DPTRACING -DSTATIC_LIBS_USED"
    H323_LIBS="-lh323_FreeBSD_x86_r_s -lpt_FreeBSD_x86_r_s -pthread -lssl -lcrypto"
    ;;

  darwin* )
    H323_CFLAGS="-DP_MACOSX $ARCH_H323_CFLAGS -DNO_LONG_DOUBLE -DPHAS_TEMPLATES -DP_USE_PRAGMA -DPTRACING -DSTATIC_LIBS_USED"
    H323_LIBS="-multiply_defined suppress -lh323_Darwin_ppc_r_s -lpt_Darwin_ppc_r_s -lssl -lcrypto -lX11 -framework AudioToolbox -framework CoreAudio -framework CoreServices"
    ;;

  *)
    # Only Linux FreeBSD and Mac OS X are supported.
    stb_cv_h323_avail=false
    ;;
esac

stb_ac_h323_libs="$H323_LIBS"
stb_ac_h323_cppflags="$H323_CFLAGS $stb_ac_ptlib_cppflags $stb_ac_h323_cppflags"

AC_CACHE_CHECK(
  [whether libH323 is available],
  stb_cv_h323_avail,
  [stb_ac_save_cppflags=$CPPFLAGS
  stb_ac_save_ldflags=$LDFLAGS
  stb_ac_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $stb_ac_h323_cppflags"
  LDFLAGS="$LDFLAGS $stb_ac_h323_ldflags"
  LIBS="$stb_ac_h323_libs $LIBS"
  AC_LANG_PUSH(C++)
  AC_TRY_LINK(
    [#include <ptlib.h>
     #include <openh323/h323.h>],
    [PTRACE(1, "moo.");],
    [stb_cv_h323_avail=true],
    [stb_cv_h323_avail=false])
  AC_LANG_POP
  CPPFLAGS=$stb_ac_save_cppflags
  LDFLAGS=$stb_ac_save_ldflags
  LIBS=$stb_ac_save_libs
])

stb_ac_h323_avail=$stb_cv_h323_avail

if $stb_ac_h323_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # STB_AC_HAVE_H323_IFELSE()

