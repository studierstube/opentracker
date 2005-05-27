############################################################################
# Usage:
#   STB_AC_HAVE_SIMVOLEON_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the Simvoleon library. If it is found, the set of
#   variables listed below are set up as described and made available to 
#   the configure script.
#
# Autoconf Variables:
# < $stb_ac_simvol_avail       true | false
# < $stb_ac_simvol_cppflags    (extra flags the preprocessor needs)
# < $stb_ac_simvol_ldflags     (extra flags the linker needs)
# < $stb_ac_simvol_libs        (link library flags the linker needs)
# < $stb_ac_simvol_rpath       (library directory if not in linker search path)
#
# Author:
#   Tamer Fahmy <tamer@tammura.at>
#
# TODO:
#

AC_DEFUN([STB_AC_HAVE_SIMVOLEON_IFELSE], [
AC_PREREQ([2.53])

# official variables
stb_ac_simvol_cppflags=
stb_ac_simvol_ldflags=
stb_ac_simvol_libs=
stb_ac_simvol_root=
stb_ac_simvol_rpath=

# internal variables
stb_ac_simvol_extrapath=

AC_ARG_WITH([simvoleon],
  AC_HELP_STRING([--with-simvoleon=DIR], [give prefix location of Simvoleon]),
  [stb_ac_simvol_extrapath=$withval], [])

stb_ac_simvol_root=$stb_ac_simvol_extrapath

stb_ac_simvol_path=$PATH
if test -z "$stb_ac_simvol_extrapath"; then ## search in --prefix path
  stb_ac_simvol_path=$stb_ac_simvol_path:$prefix/bin
else                                    ## search in --with-simvoleon path
  case "$stb_ac_simvoleon_extrapath" in 	 
     [[\\/]]* | ?:[[\\/]]* ) : ;; 	 
     * ) stb_ac_simvol_extrapath="`pwd`/$stb_ac_simvol_extrapath"
  esac
  stb_ac_simvol_path=$stb_ac_simvol_extrapath/bin:$stb_ac_path
  stb_ac_simvol_rpath="-R $stb_ac_simvol_extrapath/lib"
fi

AC_PATH_PROG(stb_ac_simvol_configcmd, simvoleon-config, false, $stb_ac_simvol_path)
if $stb_ac_simvol_configcmd; then
  stb_ac_simvol_cppflags=`$stb_ac_simvol_configcmd --cppflags`
  stb_ac_simvol_ldflags=`$stb_ac_simvol_configcmd --ldflags`
  stb_ac_simvol_libs=`$stb_ac_simvol_configcmd --libs`
  stb_ac_simvol_version=`$stb_ac_simvol_configcmd --version`

  AC_CACHE_CHECK(
    [whether libSimVoleon is available],
    stb_cv_simvol_avail,
    [stb_ac_save_cppflags=$CPPFLAGS
    stb_ac_save_ldflags=$LDFLAGS
    stb_ac_save_libs=$LIBS
    CPPFLAGS="$CPPFLAGS $CXXFLAGS $stb_ac_simvol_cppflags"
    LDFLAGS="$LDFLAGS $stb_ac_simvol_ldflags"
    LIBS="$stb_ac_simvol_libs $LIBS"
    AC_LANG_PUSH(C++)
    AC_TRY_LINK(
      [#include <VolumeViz/nodes/SoVolumeRendering.h>],
      [SoVolumeRendering::init();],
      [stb_cv_simvol_avail=true],
      [stb_cv_simvol_avail=false])
    AC_LANG_POP
    CPPFLAGS=$stb_ac_save_cppflags
    LDFLAGS=$stb_ac_save_ldflags
    LIBS=$stb_ac_save_libs
  ])
  
  stb_ac_simvol_avail=$stb_cv_simvol_avail
fi
  
if $stb_ac_simvol_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # STB_AC_HAVE_SIMVOLEON_IFELSE()

