############################################################################
# Usage:
#   STB_AC_HAVE_ACE_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the ACE development system.  If it is found,
#   the set of variables listed below are set up as described and made
#   available to the configure script.
#
# Autoconf Variables:
# < $stb_ac_ace_avail       true | false
# < $stb_ac_ace_cppflags    (extra flags the preprocessor needs)
# < $stb_ac_ace_ldflags     (extra flags the linker needs)
# < $stb_ac_ace_libs        (link library flags the linker needs)
# < $stb_ac_ace_rpath       (library directory if not in linker search path)
#
# Author:
#   Tamer Fahmy <tamer@tammura.at>
#
# TODO:
#

AC_DEFUN([STB_AC_HAVE_ACE_IFELSE], [
AC_PREREQ([2.53])

# official variables
stb_ac_ace_cppflags=
stb_ac_ace_ldflags=
stb_ac_ace_libs=
stb_ac_ace_root=
stb_ac_ace_rpath=

# internal variables
stb_ac_ace_extrapath=

AC_ARG_WITH([ace],
  AC_HELP_STRING([--with-ace=DIR], [give prefix location of ACE]),
  [stb_ac_ace_extrapath=$withval], [])

if test -z "$stb_ac_ace_extrapath"; then ## search in --prefix
  stb_ac_ace_extrapath=$prefix/ACE_wrappers
else
  case "$stb_ac_ace_extrapath" in
    [[\\/]]* | ?:[[\\/]]* ) : 
        stb_ac_ace_rpath="-R $stb_ac_ace_extrapath/ace" ;;
    * ) stb_ac_ace_extrapath=`pwd`/$stb_ac_ace_extrapath
        stb_ac_ace_rpath="-R $stb_ac_ace_extrapath/ace" ;;
  esac
fi

stb_ac_ace_root=$stb_ac_ace_extrapath

stb_ac_ace_cppflags="-I$stb_ac_ace_extrapath/"
stb_ac_ace_ldflags="-L$stb_ac_ace_extrapath/ace"
stb_ac_ace_libs="-lACE"

AC_CACHE_CHECK(
  [whether libACE is available],
  stb_cv_ace_avail,
  [stb_ac_save_cppflags=$CPPFLAGS
  stb_ac_save_ldflags=$LDFLAGS
  stb_ac_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $stb_ac_ace_cppflags"
  LDFLAGS="$LDFLAGS $stb_ac_ace_ldflags"
  LIBS="$stb_ac_ace_libs $LIBS"
  AC_LANG_PUSH(C++)
  AC_TRY_LINK(
    [#include <ace/Reactor.h>],
    [ACE_Reactor *g_reactor; ACE_NEW_RETURN(g_reactor, ACE_Reactor, 1);],
    [stb_cv_ace_avail=true],
    [stb_cv_ace_avail=false])
  AC_LANG_POP
  CPPFLAGS=$stb_ac_save_cppflags
  LDFLAGS=$stb_ac_save_ldflags
  LIBS=$stb_ac_save_libs
])

stb_ac_ace_avail=$stb_cv_ace_avail

if $stb_ac_ace_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # STB_AC_HAVE_ACE_IFELSE()

