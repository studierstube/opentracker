############################################################################
# Usage:
#   STB_AC_HAVE_ACIS_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the ACIS library. If it is found, the set of
#   variables listed below are set up as described and made available to 
#   the configure script.
#
# Autoconf Variables:
# < $stb_ac_acis_avail       true | false
# < $stb_ac_acis_cppflags    (extra flags the preprocessor needs)
# < $stb_ac_acis_ldflags     (extra flags the linker needs)
# < $stb_ac_acis_libs        (link library flags the linker needs)
# < $stb_ac_acis_rpath       (library directory if not in linker search path)
#
# Author:
#   Tamer Fahmy <tamer@tammura.at>
#
# TODO:
#

AC_DEFUN([STB_AC_HAVE_ACIS_IFELSE], [
AC_PREREQ([2.53])

# official variables
stb_ac_acis_cppflags=
stb_ac_acis_ldflags=
stb_ac_acis_libs=
stb_ac_acis_root=
stb_ac_acis_rpath=

# internal variables
stb_ac_acis_extrapath=

AC_ARG_WITH([acis],
  AC_HELP_STRING([--with-acis=DIR], [give prefix location of Acis]),
  [stb_ac_acis_extrapath=$withval], [])

stb_ac_acis_root=$stb_ac_acis_extrapath

stb_ac_acis_path=$PATH
if test -z "$stb_ac_acis_extrapath"; then ## search in --prefix path
  stb_ac_acis_path=$stb_ac_acis_path:$prefix/bin
  stb_ac_acis_cppflags="-I$ac_default_prefix/include"
  stb_ac_acis_ldflags="-L$ac_default_prefix/bin/linux_so"
  stb_ac_acis_root=$ac_default_prefix
else                                    ## search in --with-acis path
  case "$stb_ac_acis_extrapath" in 	 
     [[\\/]]* | ?:[[\\/]]* ) : ;;
     * ) stb_ac_acis_extrapath="`pwd`/$stb_ac_acis_extrapath"
  esac
  stb_ac_acis_path=$stb_ac_acis_extrapath/bin:$stb_ac_path
  stb_ac_acis_cppflags="-I$stb_ac_acis_extrapath/include"
  stb_ac_acis_ldflags="-L$stb_ac_acis_extrapath/bin/linux_so"
  stb_ac_acis_rpath="-R $stb_ac_acis_extrapath/bin/linux_so"
fi

stb_ac_acis_libs="-lSpaBase -lSpaACIS -lSpaAVis"

AC_CACHE_CHECK(
  [whether libAcis is available],
  stb_cv_acis_avail,
  [stb_ac_save_cppflags=$CPPFLAGS
  stb_ac_save_ldflags=$LDFLAGS
  stb_ac_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $CXXFLAGS $stb_ac_acis_cppflags"
  LDFLAGS="$LDFLAGS $stb_ac_acis_ldflags"
  LIBS="$stb_ac_acis_libs $LIBS"
  AC_LANG_PUSH(C++)
  AC_TRY_LINK(
    [#include <edge.hxx>],
    [EDGE *edge = new EDGE;],
    [stb_cv_acis_avail=true],
    [stb_cv_acis_avail=false])
  AC_LANG_POP
  CPPFLAGS=$stb_ac_save_cppflags
  LDFLAGS=$stb_ac_save_ldflags
  LIBS=$stb_ac_save_libs
])

stb_ac_acis_avail=$stb_cv_acis_avail
  
if $stb_ac_acis_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # STB_AC_HAVE_ACIS_IFELSE()

