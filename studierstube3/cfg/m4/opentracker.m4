############################################################################
# Usage:
#   STB_AC_HAVE_OPENTRACKER_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the Opentracker library. If it is found, the set of
#   variables listed below are set up as described and made available to 
#   the configure script.
#
# Autoconf Variables:
# < $stb_ac_ot_avail       true | false
# < $stb_ac_ot_cppflags    (extra flags the preprocessor needs)
# < $stb_ac_ot_ldflags     (extra flags the linker needs)
# < $stb_ac_ot_libs        (link library flags the linker needs)
# < $stb_ac_ot_rpath       (library directory if not in linker search path)
#
# Author:
#   Tamer Fahmy <tamer@tammura.at>
#
# TODO:
#

AC_DEFUN([STB_AC_HAVE_OPENTRACKER_IFELSE], [
AC_PREREQ([2.53])

# official variables
stb_ac_ot_cppflags=
stb_ac_ot_ldflags=
stb_ac_ot_libs=
stb_ac_ot_root=
stb_ac_ot_rpath=

# internal variables
stb_ac_ot_extrapath=

AC_ARG_WITH([opentracker],
  AC_HELP_STRING([--with-opentracker=DIR], [give prefix location of OpenTracker]),
  [stb_ac_ot_extrapath=$withval], [])

stb_ac_ot_root=$stb_ac_ot_extrapath

stb_ac_ot_path=$PATH
if test -z "$stb_ac_ot_extrapath"; then ## search in --prefix path
  stb_ac_ot_path=$stb_ac_ot_path:$prefix/bin
else                                    ## search in --with-opentracker path
  case "$stb_ac_opentracker_extrapath" in 	 
     [[\\/]]* | ?:[[\\/]]* ) : ;; 	 
     * ) stb_ac_ot_extrapath="`pwd`/$stb_ac_ot_extrapath"
  esac
  stb_ac_ot_path=$stb_ac_ot_extrapath/bin:$stb_ac_path
  stb_ac_ot_rpath="-R $stb_ac_ot_extrapath/lib"
fi

AC_PATH_PROG(stb_ac_ot_configcmd, opentracker-config, false, $stb_ac_ot_path)
if $stb_ac_ot_configcmd; then
  stb_ac_ot_cppflags=`$stb_ac_ot_configcmd --cppflags`
  stb_ac_ot_ldflags=`$stb_ac_ot_configcmd --ldflags`
  stb_ac_ot_libs=`$stb_ac_ot_configcmd --libs`
  stb_ac_ot_version=`$stb_ac_ot_configcmd --version`

  AC_CACHE_CHECK(
    [whether libOpenTracker is available],
    stb_cv_ot_avail,
    [stb_ac_save_cppflags=$CPPFLAGS
    stb_ac_save_ldflags=$LDFLAGS
    stb_ac_save_libs=$LIBS
    CPPFLAGS="$CPPFLAGS $CXXFLAGS $stb_ac_ot_cppflags"
    LDFLAGS="$LDFLAGS $stb_ac_ot_ldflags"
    LIBS="$stb_ac_ot_libs $LIBS"
    AC_LANG_PUSH(C++)
    AC_TRY_LINK(
      [#include <OpenTracker.h>],
      [Context context; initializeContext(context);],
      [stb_cv_ot_avail=true],
      [stb_cv_ot_avail=false])
    AC_LANG_POP
    CPPFLAGS=$stb_ac_save_cppflags
    LDFLAGS=$stb_ac_save_ldflags
    LIBS=$stb_ac_save_libs
  ])
  
  stb_ac_ot_avail=$stb_cv_ot_avail
fi
  
if $stb_ac_ot_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # STB_AC_HAVE_OPENTRACKER_IFELSE()

