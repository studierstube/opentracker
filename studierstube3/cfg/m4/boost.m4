############################################################################
# Usage:
#   STB_AC_HAVE_BOOST_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the BOOST library. If it is found, the set of
#   variables listed below are set up as described and made available to 
#   the configure script.
#
# Autoconf Variables:
# < $stb_ac_boost_avail       true | false
# < $stb_ac_boost_cppflags    (extra flags the preprocessor needs)
# < $stb_ac_boost_ldflags     (extra flags the linker needs)
# < $stb_ac_boost_libs        (link library flags the linker needs)
# < $stb_ac_boost_rpath       (library directory if not in linker search path)
#
# Author:
#   Tamer Fahmy <tamer@tammura.at>
#
# TODO:
#

AC_DEFUN([STB_AC_HAVE_BOOST_IFELSE], [
AC_PREREQ([2.53])

# official variables
stb_ac_boost_cppflags=
stb_ac_boost_ldflags=
stb_ac_boost_libs=
stb_ac_boost_root=
stb_ac_boost_rpath=

# internal variables
stb_ac_boost_extrapath=

AC_ARG_WITH([boost],
  AC_HELP_STRING([--with-boost=DIR], [give prefix location of Boost]),
  [stb_ac_boost_extrapath=$withval], [])

stb_ac_boost_root=$stb_ac_boost_extrapath

stb_ac_boost_path=$PATH
if test -z "$stb_ac_boost_extrapath"; then
  if test -d "/usr/include/boost"; then ## see if it exists in /usr
    stb_ac_boost_path=$stb_ac_boost_path:$prefix/bin
    stb_ac_boost_cppflags="-I/usr/include/boost"
    stb_ac_boost_ldflags="-L/usr/lib/boost"
    stb_ac_boost_root="/usr"
  else                                         ## search in --prefix path
    stb_ac_boost_path=$stb_ac_boost_path:$prefix/bin
    stb_ac_boost_cppflags="-I$ac_default_prefix/include/boost"
    stb_ac_boost_ldflags="-L$ac_default_prefix/lib/boost"
    stb_ac_boost_root=$ac_default_prefix
  fi
else                                    ## search in --with-boost path
  case "$stb_ac_boost_extrapath" in 	 
     [[\\/]]* | ?:[[\\/]]* ) : ;;
     * ) stb_ac_boost_extrapath="`pwd`/$stb_ac_boost_extrapath"
  esac
  stb_ac_boost_path=$stb_ac_boost_extrapath/bin:$stb_ac_path
  stb_ac_boost_cppflags="-I$stb_ac_boost_extrapath/include/boost"
  stb_ac_boost_ldflags="-L$stb_ac_boost_extrapath/lib"
  stb_ac_boost_rpath="-R $stb_ac_boost_extrapath/lib"
fi

stb_ac_boost_libs="-lboost_python -lboost_regex -lboost_thread"

AC_CACHE_CHECK(
  [whether libBoost is available],
  stb_cv_boost_avail,
  [stb_ac_save_cppflags=$CPPFLAGS
  stb_ac_save_ldflags=$LDFLAGS
  stb_ac_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $CXXFLAGS $stb_ac_boost_cppflags"
  LDFLAGS="$LDFLAGS $stb_ac_boost_ldflags"
  LIBS="$stb_ac_boost_libs $LIBS"
  AC_LANG_PUSH(C++)
  AC_TRY_COMPILE(
    [#include <boost/version.hpp>],
    [],
    [stb_cv_boost_avail=true],
    [stb_cv_boost_avail=false])
  AC_LANG_POP
  CPPFLAGS=$stb_ac_save_cppflags
  LDFLAGS=$stb_ac_save_ldflags
  LIBS=$stb_ac_save_libs
])

stb_ac_boost_avail=$stb_cv_boost_avail
  
if $stb_ac_boost_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # STB_AC_HAVE_BOOST_IFELSE()

