# Process this file with autoconf to produce a configure script.
AC_INIT([Opentracker], [1.3], [opentracker@studierstube.org])

AC_CONFIG_AUX_DIR(cfg)
AC_CONFIG_SRCDIR([src/core/Node.cxx])

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

OT_VERSION="1.3"
AC_SUBST([OT_VERSION])

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
AC_PROG_LIBTOOL
AC_PROG_INSTALL

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_STDBOOL

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_C_CONST
AC_C_INLINE
AC_HEADER_TIME
AC_STRUCT_TM
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL
AC_CXX_TYPENAME
AC_CXX_HAVE_SSTREAM

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([floor pow sqrt])

# Check for curses.
AC_CHECK_CURSES

if test x"$has_curses" = xfalse; then
  AC_MSG_ERROR([a curses library was not found!])
fi

AC_SUBST([CURSES_INCLUDEDIR])
AC_SUBST([CURSES_LIBS])


# Check for ACE.
STB_AC_HAVE_ACE_IFELSE([], [AC_MSG_ERROR([cannot link to the ACE library. consider --with-ace=path_to_ace])])

AC_SUBST([stb_ac_ace_cppflags])
AC_SUBST([stb_ac_ace_ldflags])
AC_SUBST([stb_ac_ace_libs])
AC_SUBST([stb_ac_ace_rpath])

SIM_AC_CONFIGURATION_SETTING([ACE root directory], [$stb_ac_ace_root])

# Check for Xerces.
STB_AC_HAVE_XERCES_IFELSE([],
    [AC_MSG_ERROR([cannot link to the xerces library. consider --with-xerces=path_to_xerces
     and make sure that you got the right xerces version as listed in INSTALL.])])

AC_SUBST([stb_ac_xerces_cppflags])
AC_SUBST([stb_ac_xerces_ldflags])
AC_SUBST([stb_ac_xerces_libs])
AC_SUBST([stb_ac_xerces_rpath])

SIM_AC_CONFIGURATION_SETTING([Xerces root directory], [$stb_ac_xerces_root])

# Check for ARToolkit.
HAVE_FEATURE_ARTOOLKIT=0

STB_AC_HAVE_ARTOOLKIT_IFELSE([], [])

if test x"$stb_ac_artoolkit_avail" = xtrue; then
  AC_DEFINE(USE_ARTOOLKIT, 1, [Enable the use of the ARToolkit library])
  HAVE_FEATURE_ARTOOLKIT=1
  AC_SUBST([HAVE_FEATURE_ARTOOLKIT])
else
  stb_ac_artoolkit_cppflags=
  stb_ac_artoolkit_ldflags=
  stb_ac_artoolkit_libs=
  stb_ac_artoolkit_root="not found"
fi

AC_SUBST([stb_ac_artoolkit_cppflags])
AC_SUBST([stb_ac_artoolkit_ldflags])
AC_SUBST([stb_ac_artoolkit_libs])
AC_SUBST([stb_ac_artoolkit_rpath])

SIM_AC_CONFIGURATION_SETTING([ARToolkit root directory], [$stb_ac_artoolkit_root])

# Check for the use of the speech recognition
HAVE_FEATURE_SAPISPEECH=0

AC_ARG_ENABLE([speech],
              AC_HELP_STRING([--enable-speech], [enable the Speech recognition (default=no)]),
              opt_speech=$enable_speech, opt_speech=no)
if test x"$opt_speech" = xyes; then
  AC_DEFINE(USE_SAPISPEECH, 1, [Enable the use of the speech recognition])
  HAVE_FEATURE_SAPISPEECH=1
  AC_SUBST([HAVE_FEATURE_SAPISPEECH])
fi
SIM_AC_CONFIGURATION_SETTING([Enable speech recognition], [$opt_speech])

# Check for the use of the joystick
HAVE_FEATURE_JOYSTICK=0

AC_ARG_ENABLE([joystick],
              AC_HELP_STRING([--enable-joystick], [enable the use of joysticks (default=no)]),
              opt_joystick=$enable_joystick, opt_joystick=no)
if test x"$opt_joystick" = xyes; then
  AC_DEFINE(USE_JOYSTICK, 1, [Enable the use of joysticks])
  HAVE_FEATURE_JOYSTICK=1
  AC_SUBST([HAVE_FEATURE_JOYSTICK])
fi
SIM_AC_CONFIGURATION_SETTING([Enable joysticks], [$opt_joystick])

# Check for the use of OTQt
HAVE_FEATURE_OTQT=0
AC_ARG_ENABLE([otqt],
              AC_HELP_STRING([--enable-otqt], [enable the OTQt module (default=no)]),
              opt_otqt=$enable_otqt, opt_otqt=no)
SIM_AC_CONFIGURATION_SETTING([Enable OTQt], [$opt_otqt])
AM_CONDITIONAL([WANT_OTQT], [test x"$opt_otqt" = xyes])
if test x"$opt_otqt" = xyes; then
  # Check for Qt
  gw_CHECK_QT
  SIM_AC_CONFIGURATION_SETTING([Qt root directory], [$QTDIR])
  # autoconf settings
  AC_DEFINE(USE_OTQT, 1, [Enable the use of OTQt])
  HAVE_FEATURE_OTQT=1
  AC_SUBST([HAVE_FEATURE_OTQT])
fi

AC_SUBST([LIBS])
AC_SUBST([CXXFLAGS])
AC_SUBST([CPPFLAGS])
AC_SUBST([LDFLAGS])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/common/Makefile
                 src/core/Makefile
                 src/input/Makefile
                 src/tool/Makefile
                 src/misc/Makefile
                 src/misc/xml/Makefile
                 src/network/Makefile
                 src/otqt/Makefile
                 extras/Makefile
                 extras/intersense/Makefile
                 extras/intersense/unix/Makefile
                 extras/newmat10/Makefile
                 extras/tclap/Makefile
                 lib/Makefile
                 bin/Makefile
                 data/Makefile])

AC_OUTPUT

SIM_AC_CONFIGURATION_SUMMARY

echo ""
echo "Now, run 'make', cross your fingers, start a prayer and get yourself"
echo "a cup of coffee to build $PACKAGE."
echo ""
