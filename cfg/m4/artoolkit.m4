############################################################################
# Usage:
#   STB_AC_HAVE_ARTOOLKIT_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the Artoolkit library and headers. If it is found,
#   the set of variables listed below are set up as described and made
#   available to the configure script.
#
# Autoconf Variables:
# < $stb_ac_artoolkit_avail       true | false
# < $stb_ac_artoolkit_cppflags    (extra flags the preprocessor needs)
# < $stb_ac_artoolkit_ldflags     (extra flags the linker needs)
# < $stb_ac_artoolkit_libs        (link library flags the linker needs)
# < $stb_ac_artoolkit_rpath       (library directory if not in linker search path)
#
# Author:
#   Tamer Fahmy <tamer@tammura.at>
#
# TODO:
#

AC_DEFUN([STB_AC_HAVE_ARTOOLKIT_IFELSE], [
AC_PREREQ([2.53])

# official variables
stb_ac_artoolkit_cppflags=
stb_ac_artoolkit_ldflags=
stb_ac_artoolkit_libs=
stb_ac_artoolkit_root=
stb_ac_artoolkit_rpath=

# internal variables
stb_ac_artoolkit_extrapath=

AC_ARG_WITH([artoolkit],
  AC_HELP_STRING([--with-artoolkit=DIR], [give prefix location of ARToolkit]),
  [stb_ac_artoolkit_extrapath=$withval], [])

if test -z "$stb_ac_artoolkit_extrapath"; then ## search in --prefix
  stb_ac_artoolkit_extrapath=$prefix/lib
else
  case "$stb_ac_artoolkit_extrapath" in
    [[\\/]]* | ?:[[\\/]]* ) : ;;
    * ) stb_ac_artoolkit_extrapath="`pwd`/$stb_ac_artoolkit_extrapath"
        stb_ac_artoolkit_rpath="-R $stb_ac_artoolkit_extrapath/lib" ;;
  esac
fi

stb_ac_artoolkit_root=$stb_ac_artoolkit_extrapath

stb_ac_artoolkit_cppflags="-I$stb_ac_artoolkit_extrapath/include"
stb_ac_artoolkit_ldflags="-L$stb_ac_artoolkit_extrapath/lib"
stb_ac_artoolkit_libs="-lAR -lARgsub -lARgsubUtil -lARMulti -lARvideo"

AC_CACHE_CHECK(
  [whether ARToolkit is available],
  stb_cv_artoolkit_avail,
  [stb_ac_save_cppflags=$CPPFLAGS
  stb_ac_save_ldflags=$LDFLAGS
  stb_ac_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $stb_ac_artoolkit_cppflags"
  LDFLAGS="$LDFLAGS $stb_ac_artoolkit_ldflags"
  LIBS="$stb_ac_artoolkit_libs $LIBS"
  AC_LANG_PUSH(C++)
  AC_TRY_LINK(
    [#include <AR/param.h>],
    [ARParam cparam;],
    [stb_cv_artoolkit_avail=true],
    [stb_cv_artoolkit_avail=false])
  AC_LANG_POP
  CPPFLAGS=$stb_ac_save_cppflags
  LDFLAGS=$stb_ac_save_ldflags
  LIBS=$stb_ac_save_libs
])

stb_ac_artoolkit_avail=$stb_cv_artoolkit_avail

if $stb_ac_artoolkit_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # STB_AC_HAVE_ARTOOLKIT_IFELSE()

