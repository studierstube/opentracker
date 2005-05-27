############################################################################
# Usage:
#   STB_AC_HAVE_VTK_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the VTK library. If it is found, the set of
#   variables listed below are set up as described and made available to 
#   the configure script.
#
# Autoconf Variables:
# < $stb_ac_vtk_avail       true | false
# < $stb_ac_vtk_cppflags    (extra flags the preprocessor needs)
# < $stb_ac_vtk_ldflags     (extra flags the linker needs)
# < $stb_ac_vtk_libs        (link library flags the linker needs)
# < $stb_ac_vtk_rpath       (library directory if not in linker search path)
#
# Author:
#   Tamer Fahmy <tamer@tammura.at>
#
# TODO:
#

AC_DEFUN([STB_AC_HAVE_VTK_IFELSE], [
AC_PREREQ([2.53])

# official variables
stb_ac_vtk_cppflags=
stb_ac_vtk_ldflags=
stb_ac_vtk_libs=
stb_ac_vtk_root=
stb_ac_vtk_rpath=

# internal variables
stb_ac_vtk_extrapath=

AC_ARG_WITH([vtk],
  AC_HELP_STRING([--with-vtk=DIR], [give prefix location of Vtk]),
  [stb_ac_vtk_extrapath=$withval], [])

stb_ac_vtk_root=$stb_ac_vtk_extrapath

stb_ac_vtk_path=$PATH
if test -z "$stb_ac_vtk_extrapath"; then ## search in --prefix path
  stb_ac_vtk_path=$stb_ac_vtk_path:$prefix/bin
  stb_ac_vtk_cppflags="-I$ac_default_prefix/include/vtk"
  stb_ac_vtk_ldflags="-L$ac_default_prefix/lib/vtk"
  stb_ac_vtk_root=$ac_default_prefix
else                                    ## search in --with-vtk path
  case "$stb_ac_vtk_extrapath" in 	 
     [[\\/]]* | ?:[[\\/]]* ) : ;;
     * ) stb_ac_vtk_extrapath="`pwd`/$stb_ac_vtk_extrapath"
  esac
  stb_ac_vtk_path=$stb_ac_vtk_extrapath/bin:$stb_ac_path
  stb_ac_vtk_cppflags="-I$stb_ac_vtk_extrapath/include/vtk"
  stb_ac_vtk_ldflags="-L$stb_ac_vtk_extrapath/lib/vtk"
  stb_ac_vtk_rpath="-R $stb_ac_vtk_extrapath/lib/vtk"
fi

stb_ac_vtk_libs="-lvtkCommon -lvtkIO -lvtkGraphics -lvtkFiltering -lvtkRendering -lvtkHybrid"

AC_CACHE_CHECK(
  [whether libVtk is available],
  stb_cv_vtk_avail,
  [stb_ac_save_cppflags=$CPPFLAGS
  stb_ac_save_ldflags=$LDFLAGS
  stb_ac_save_libs=$LIBS
  CPPFLAGS="$CPPFLAGS $CXXFLAGS $stb_ac_vtk_cppflags"
  LDFLAGS="$LDFLAGS $stb_ac_vtk_ldflags"
  LIBS="$stb_ac_vtk_libs $LIBS"
  AC_LANG_PUSH(C++)
  AC_TRY_LINK(
    [#include <vtkConeSource.h>],
    [vtkConeSource *vcs = vtkConeSource::New();],
    [stb_cv_vtk_avail=true],
    [stb_cv_vtk_avail=false])
  AC_LANG_POP
  CPPFLAGS=$stb_ac_save_cppflags
  LDFLAGS=$stb_ac_save_ldflags
  LIBS=$stb_ac_save_libs
])

stb_ac_vtk_avail=$stb_cv_vtk_avail
  
if $stb_ac_vtk_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # STB_AC_HAVE_VTK_IFELSE()

