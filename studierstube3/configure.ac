# Process this file with autoconf to produce a configure script.
AC_INIT([Studierstube], [3.2], [studierstube@studierstube.org])

AC_CONFIG_AUX_DIR([cfg])
AC_CONFIG_SRCDIR([src/stbapi/util/ivutil.cxx])
AC_CONFIG_SUBDIRS(libltdl)

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

OT_VERSION="3.2"
AC_SUBST([STB_VERSION])

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
SIM_AC_COMPILER_WARNINGS
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_LANG(C++)

# AM_DISABLE_SHARED
# AM_DISABLE_STATIC
AC_DISABLE_FAST_INSTALL
# libtdl checks and setup
AC_LIBTOOL_DLOPEN
AC_LIBLTDL_CONVENIENCE
AC_PROG_LIBTOOL
AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)
AC_SUBST(LIBADD_DL)

# Checks for libraries.
AC_CHECK_LIB([z], [deflate], [], [AC_MSG_ERROR([cannot link to the zlib library.])])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([sys/prctl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_HEADER_TIME
AC_STRUCT_TM
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL
AC_CXX_TYPENAME

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL

# holds the available videobackground implementations detected
stb_videobg_msg="NONE"

# darwin specific checks
case $host_os in
darwin*)
  # Check for the availability of fink
  AC_MSG_CHECKING([if fink is available])
  fink_avail="no"
  if test -d /sw/include && test -d /sw/lib; then
    fink_avail="yes"
    CPPFLAGS="$CPPFLAGS -I/sw/include -I/usr/X11R6/include"
    LDFLAGS="$LDFLAGS -L/sw/lib -L/usr/X11R6/lib"
  fi
  AC_MSG_RESULT([$fink_avail])
  # Check for dlcompat and bail out if missing
  AC_CHECK_LIB([dl], [dlopen], [], [AC_MSG_ERROR([cannot link to the dlcompat library.])])
  ;;
esac

# Check for ACE.
STB_AC_HAVE_ACE_IFELSE([], [AC_MSG_ERROR([cannot link to the ACE library. consider --with-ace=path_to_ace])])

AC_SUBST([stb_ac_ace_cppflags])
AC_SUBST([stb_ac_ace_ldflags])
AC_SUBST([stb_ac_ace_libs])
AC_SUBST([stb_ac_ace_rpath])

SIM_AC_CONFIGURATION_SETTING([ACE root directory], [$stb_ac_ace_root])

# Check for OpenTracker.
STB_AC_HAVE_OPENTRACKER_IFELSE([], [AC_MSG_ERROR([cannot link to the OpenTracker library. consider --with-opentracker=path_to_opentracker])])

AC_SUBST([stb_ac_ot_cppflags])
AC_SUBST([stb_ac_ot_ldflags])
AC_SUBST([stb_ac_ot_libs])
AC_SUBST([stb_ac_ot_rpath])

SIM_AC_CONFIGURATION_SETTING([OpenTracker root directory], [$stb_ac_opentracker_root])

# Check for VTK.
STB_AC_HAVE_VTK_IFELSE([], [])

if test x"$stb_ac_vtk_avail" = xtrue; then
  AC_DEFINE(USE_VTK, 1, [Enable the use of the VTK library])
  HAVE_FEATURE_VTK=1
  AC_SUBST([HAVE_FEATURE_VTK])
else
  stb_ac_vtk_cppflags=
  stb_ac_vtk_ldflags=
  stb_ac_vtk_libs=
  stb_ac_vtk_root="not found"
fi

AC_SUBST([stb_ac_vtk_cppflags])
AC_SUBST([stb_ac_vtk_ldflags])
AC_SUBST([stb_ac_vtk_libs])
AC_SUBST([stb_ac_vtk_rpath])

AM_CONDITIONAL(USE_VTK, test x"$stb_ac_vtk_avail" = xtrue)
SIM_AC_CONFIGURATION_SETTING([VTK root directory], [$stb_ac_vtk_root])

# Check for BOOST.
STB_AC_HAVE_BOOST_IFELSE([], [])

if test x"$stb_ac_boost_avail" = xtrue; then
  AC_DEFINE(USE_BOOST, 1, [Enable the use of the boost library])
  HAVE_FEATURE_BOOST=1
  AC_SUBST([HAVE_FEATURE_BOOST])
else
  stb_ac_boost_cppflags=
  stb_ac_boost_ldflags=
  stb_ac_boost_libs=
  stb_ac_boost_root="not found"
fi

AC_SUBST([stb_ac_boost_cppflags])
AC_SUBST([stb_ac_boost_ldflags])
AC_SUBST([stb_ac_boost_libs])
AC_SUBST([stb_ac_boost_rpath])

AM_CONDITIONAL(USE_BOOST, test x"$stb_ac_boost_avail" = xtrue)
SIM_AC_CONFIGURATION_SETTING([BOOST root directory], [$stb_ac_boost_root])

# Check for ACIS.
STB_AC_HAVE_ACIS_IFELSE([], [])

if test x"$stb_ac_acis_avail" = xtrue; then
  AC_DEFINE(USE_ACIS, 1, [Enable the use of the ACIS library])
  HAVE_FEATURE_ACIS=1
  AC_SUBST([HAVE_FEATURE_ACIS])
else
  stb_ac_acis_cppflags=
  stb_ac_acis_ldflags=
  stb_ac_acis_libs=
  stb_ac_acis_root="not found"
fi

AC_SUBST([stb_ac_acis_cppflags])
AC_SUBST([stb_ac_acis_ldflags])
AC_SUBST([stb_ac_acis_libs])
AC_SUBST([stb_ac_acis_rpath])

AM_CONDITIONAL(USE_ACIS, test x"$stb_ac_acis_avail" = xtrue)
SIM_AC_CONFIGURATION_SETTING([ACIS root directory], [$stb_ac_acis_root])

# check if OpenTracker has been compiled in with ARToolKit support
# and only then check for the availability of OpenH323
if `$stb_ac_ot_configcmd --have-feature artoolkit`; then
  AC_DEFINE(USE_ARTOOLKIT, 1, [Enable the use of the libccvt])
  stb_videobg_msg="$stb_videobg_msg OT "

  # Check for OpenH323
  STB_AC_HAVE_H323_IFELSE([], [])
fi

if test x"$stb_ac_h323_avail" = xtrue; then
  AC_DEFINE(USE_H323, 1, [Enable the use of the H323 library])
  HAVE_FEATURE_H323=1
  AC_SUBST([HAVE_FEATURE_H323])
  stb_videobg_msg="$stb_videobg_msg H323 "
else
  stb_ac_h323_cppflags=
  stb_ac_h323_ldflags=
  stb_ac_h323_libs=
  stb_ac_h323_root="not found"
fi

AC_SUBST([stb_ac_h323_cppflags])
AC_SUBST([stb_ac_h323_ldflags])
AC_SUBST([stb_ac_h323_libs])
AC_SUBST([stb_ac_h323_rpath])

AM_CONDITIONAL(USE_H323, test x"$stb_ac_h323_avail" = xtrue)
SIM_AC_CONFIGURATION_SETTING([OpenH323 root directory], [$stb_ac_h323_root])

# Check for fmod
stb_ac_fmod_libs=
AC_CHECK_LIB(fmod, FSOUND_PlaySound, stb_ac_fmod_libs="-lfmod")
AC_SUBST([stb_ac_fmod_libs])

# Check for the use of DIV
AC_ARG_ENABLE([div],
              AC_HELP_STRING([--enable-div], [enable Distributed Inventor (default=yes)]),
              stb_div=$enable_div, stb_div=yes)
if test "$stb_div" = yes; then
  AC_DEFINE(USE_DIV, 1, [Enable the use of the Distributed Inventor])
fi
SIM_AC_CONFIGURATION_SETTING([Enable Distributed Inventor], [$stb_div])

# Check for the use of libccvt
AC_CHECK_LIB([ccvt], [ccvt_420p_bgr24], [stb_libccvt="yes"], [])

if test "$stb_libccvt" = yes; then
  AC_DEFINE(PC_USBCAM, 1, [Enable the use of the USB background])
  stb_videobg_msg="$stb_videobg_msg USB "
fi

AM_CONDITIONAL(USE_LIBCCVT, test "$stb_libccvt" = yes)

# Check for the use of libdv
AC_CHECK_LIB([dv], [open_1394_driver], [stb_libdv="yes"], [])

if test "$stb_libdv" = yes; then
  LIBS="$LIBS -ldv"
  AC_DEFINE(USE_IEEE1394, 1, [Enable the use of the IEEE1394 background])
  stb_videobg_msg="$stb_videobg_msg IEEE1394(DV) "
fi

AM_CONDITIONAL(USE_IEEE1394, test "$stb_libdv" = yes)

# Check for the use of libdc1394_control
AC_CHECK_LIB([dc1394_control], [dc1394_get_camera_nodes], [stb_libdc1394_control="yes"], [])

if test "$stb_libdc1394_control" = yes; then
  LIBS="$LIBS -ldc1394_control -lraw1394"
  AC_DEFINE(USE_DC1394, 1, [Enable the use of DC1394 background])
  stb_videobg_msg="$stb_videobg_msg IEEE1394(DC) "
fi

AM_CONDITIONAL(USE_DC1394, test "$stb_libdc1394_control" = yes)

SIM_AC_CONFIGURATION_SETTING([Videobackgrounds available], [$stb_videobg_msg])

# Check for a SoGUI binding.
sim_ac_gui_default=soqt

test x${with_qt+set} = xset && sim_ac_gui_default=soqt
test x${with_gtk+set} = xset && sim_ac_gui_default=sogtk
test x${with_xt+set} = xset && sim_ac_gui_default=soxt

AC_ARG_WITH(
  soqt,
  AC_HELP_STRING([--with-soqt], [build against the SoQt toolkit]),
  [sim_ac_gui_default=soqt])
AC_ARG_WITH(
  sogtk,
  AC_HELP_STRING([--with-sogtk], [build against the SoGtk toolkit]),
  [sim_ac_gui_default=sogtk])
AC_ARG_WITH(
  soxt,
  AC_HELP_STRING([--with-soxt], [build against the SoXt toolkit]),
  [sim_ac_gui_default=soxt])

if test x"$sim_ac_gui_default" = xsoqt; then
  SIM_AC_HAVE_SOQT_IFELSE([],
	[SIM_AC_HAVE_SOGTK_IFELSE([],[SIM_AC_HAVE_SOXT_IFELSE([],[])])])
elif test x"$sim_ac_gui_default" = xsogtk; then
  SIM_AC_HAVE_SOGTK_IFELSE([],
	[SIM_AC_HAVE_SOQT_IFELSE([],[SIM_AC_HAVE_SOXT_IFELSE([],[])])])
elif test x"$sim_ac_gui_default" = xsoxt; then
  SIM_AC_HAVE_SOXT_IFELSE([],
	[SIM_AC_HAVE_SOQT_IFELSE([],
	[SIM_AC_HAVE_SOGTK_IFELSE([],[])])])
fi

if test x"$sim_ac_soqt_avail" = xtrue && test x"$sim_ac_soqt_desired" = xtrue; then
  AC_DEFINE(USE_SOQT, 1, [Use SoQt as default Gui binding])
  AC_SUBST([sogui_cppflags], [$sim_ac_soqt_cppflags])
  AC_SUBST([sogui_ldflags],  [$sim_ac_soqt_ldflags])
  AC_SUBST([sogui_libs],     [$sim_ac_soqt_libs])
  SIM_AC_CONFIGURATION_SETTING([SoGUI binding], [SoQt])
elif test x"$sim_ac_sogtk_avail" = xtrue && test x"$sim_ac_sogtk_desired" = xtrue; then
  AC_DEFINE(USE_SOGTK, 1, [Use SoGtk as default Gui binding])
  AC_SUBST([sogui_cppflags], [$sim_ac_sogtk_cppflags])
  AC_SUBST([sogui_ldflags],  [$sim_ac_sogtk_ldflags])
  AC_SUBST([sogui_libs],     [$sim_ac_sogtk_libs])
  SIM_AC_CONFIGURATION_SETTING([SoGUI binding], [SoGtk])
elif test x"$sim_ac_soxt_avail" = xtrue && test x"$sim_ac_soxt_desired" = xtrue; then
  AC_DEFINE(USE_SOXT, 1, [Use SoXt as default Gui binding])
  AC_SUBST([sogui_cppflags], [$sim_ac_soxt_cppflags])
  AC_SUBST([sogui_ldflags],  [$sim_ac_soxt_ldflags])
  AC_SUBST([sogui_libs],     [$sim_ac_soxt_libs])
  SIM_AC_CONFIGURATION_SETTING([SoGUI binding], [SoXt])
else
  AC_MSG_ERROR([none of the supported SoGUI bindings found. please provide at least one!])
fi

# Check for SimVoleon.
STB_AC_HAVE_SIMVOLEON_IFELSE([], [])

if test x"$stb_ac_simvol_avail" = xtrue; then
  AC_DEFINE(USE_SIMVOLEON, 1, [Enable the use of the SimVoleon library])
  HAVE_FEATURE_SIMVOLEON=1
  AC_SUBST([HAVE_FEATURE_SIMVOLEON])
else
  stb_ac_simvol_cppflags=
  stb_ac_simvol_ldflags=
  stb_ac_simvol_libs=
  stb_ac_simvol_root="not found"
fi

AC_SUBST([stb_ac_simvol_cppflags])
AC_SUBST([stb_ac_simvol_ldflags])
AC_SUBST([stb_ac_simvol_libs])
AC_SUBST([stb_ac_simvol_rpath])

AM_CONDITIONAL(USE_SIMVOLEON, test x"$stb_ac_simvol_avail" = xtrue)
SIM_AC_CONFIGURATION_SETTING([SimVoleon root directory], [$stb_ac_simvol_root])

AC_CONFIG_FILES([Makefile
                 bin/Makefile
                 bin/apps/Makefile
                 bin/apps/APRIL/Makefile
                 bin/apps/animal/Makefile
                 bin/apps/animalvc/Makefile
                 bin/apps/chess/Makefile
                 bin/apps/construct3d/Makefile
                 bin/apps/md2_art/Makefile
                 bin/apps/ocar/Makefile
                 bin/apps/stuberena/Makefile
                 bin/apps/vacuum/Makefile
                 bin/apps/vampire/Makefile
                 bin/apps/vtk/Makefile
                 bin/workspace/Makefile
                 lib/Makefile
                 src/Makefile
                 src/extensions/Makefile
                 src/extensions/FMODSound/Makefile
                 src/stbapi/Makefile
                 src/stbapi/context/Makefile
                 src/stbapi/div/Makefile
                 src/stbapi/event3d/Makefile
                 src/stbapi/interaction/Makefile
                 src/stbapi/misc/Makefile
                 src/stbapi/resource/Makefile
                 src/stbapi/resource/viewer/Makefile
                 src/stbapi/resource/viewer/video/Makefile
                 src/stbapi/seams/Makefile
                 src/stbapi/tracker/Makefile
                 src/stbapi/util/Makefile
                 src/stbapi/vc/Makefile
                 src/stbapi/widgets/Makefile
                 src/stbapi/window3d/Makefile
                 src/stbapi/workspace/Makefile
                 src/stbapi/workspace/distrManager/Makefile
                 tools/sman2/Makefile])

AC_OUTPUT

SIM_AC_CONFIGURATION_SUMMARY

echo ""
echo "Now, run 'make', cross your fingers, start a prayer and get yourself"
echo "a cup of coffee to build $PACKAGE."
echo ""
