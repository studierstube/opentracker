############################################################################
# Usage:
#   STB_AC_HAVE_XERCES_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the Xerces library and headers. If it is found,
#   the set of variables listed below are set up as described and made
#   available to the configure script.
#
# Autoconf Variables:
# < $stb_ac_xerces_avail       true | false
# < $stb_ac_xerces_cppflags    (extra flags the preprocessor needs)
# < $stb_ac_xerces_ldflags     (extra flags the linker needs)
# < $stb_ac_xerces_libs        (link library flags the linker needs)
#
# Author:
#   Tamer Fahmy <tamer@tammura.at>
#
# TODO:
#

AC_DEFUN([STB_AC_HAVE_XERCES_IFELSE], [
AC_PREREQ([2.53])

# official variables
stb_ac_xerces_cppflags=
stb_ac_xerces_ldflags=
stb_ac_xerces_libs=
stb_ac_xerces_root=

# internal variables
stb_ac_xerces_extrapath=

AC_ARG_WITH([xerces],
  AC_HELP_STRING([--with-xerces=DIR], [give prefix location of Xerces]),
  [stb_ac_xerces_extrapath=$withval], [])

test -z "$stb_ac_xerces_extrapath" &&   ## search in --prefix
  stb_ac_xerces_extrapath=$prefix/lib

case "$stb_ac_xerces_extrapath" in
  [[\\/]]* | ?:[[\\/]]* ) : ;;
  * ) stb_ac_xerces_extrapath="`pwd`/$stb_ac_xerces_extrapath" ;;
esac

stb_ac_xerces_root=$stb_ac_xerces_extrapath

stb_ac_xerces_cppflags="-I$stb_ac_xerces_extrapath/include"
stb_ac_xerces_ldflags="-L$stb_ac_xerces_extrapath/lib"
stb_ac_xerces_libs="-lxerces-c"

AC_CACHE_CHECK(
  [whether libXerces is available],
  stb_cv_xerces_avail,
  [stb_ac_save_cppflags=$CPPFLAGS
  stb_ac_save_ldflags=$LDFLAGS
  stb_ac_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $stb_ac_xerces_cppflags"
  LDFLAGS="$LDFLAGS $stb_ac_xerces_ldflags"
  LIBS="$stb_ac_xerces_libs $LIBS"
  AC_LANG_PUSH(C++)
  AC_TRY_LINK(
    [#include <xercesc/util/PlatformUtils.hpp>],
    [using namespace xercesc;XMLPlatformUtils::Initialize();],
    [stb_cv_xerces_avail=true],
    [stb_cv_xerces_avail=false])
  AC_LANG_POP
  CPPFLAGS=$stb_ac_save_cppflags
  LDFLAGS=$stb_ac_save_ldflags
  LIBS=$stb_ac_save_libs
])

stb_ac_xerces_avail=$stb_cv_xerces_avail

if $stb_ac_xerces_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # STB_AC_HAVE_XERCES_IFELSE()

