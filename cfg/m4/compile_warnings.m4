############################################################################
# Usage:
#   SIM_AC_COMPILER_WARNINGS
#
# Description:
#   Take care of making a sensible selection of warning messages
#   to turn on or off.
# 
#   Note: this macro must be placed after either AC_PROG_CC or AC_PROG_CXX
#   in the configure.in script.
# 
# Author: Morten Eriksen, <mortene@sim.no>.
# 
# TODO:
#   * [mortene:19991114] find out how to get GCC's
#     -Werror-implicit-function-declaration option to work as expected
#
#   * [larsa:20010504] rename to SIM_AC_COMPILER_WARNINGS and clean up
#     the macro

AC_DEFUN([SIM_AC_COMPILER_WARNINGS], [
AC_ARG_ENABLE(
  [warnings],
  AC_HELP_STRING([--enable-warnings],
                 [turn on warnings when compiling [[default=yes]]]),
  [case "${enableval}" in
    yes) enable_warnings=yes ;;
    no)  enable_warnings=no ;;
    *) AC_MSG_ERROR([bad value "$enableval" for --enable-warnings]) ;;
  esac],
  [enable_warnings=yes])

if test x"$enable_warnings" = x"yes"; then
  if test x"$GCC" = x"yes"; then
    SIM_AC_CC_COMPILER_OPTION([-W -Wall -Wno-unused],
                              [CFLAGS="$CFLAGS -W -Wall -Wno-unused"])
    SIM_AC_CC_COMPILER_OPTION([-Wno-multichar],
                              [CFLAGS="$CFLAGS -Wno-multichar"])
  fi

  if test x"$GXX" = x"yes"; then
    SIM_AC_CXX_COMPILER_OPTION([-W -Wall -Wno-unused],
                               [CXXFLAGS="$CXXFLAGS -W -Wall -Wno-unused"])
    SIM_AC_CXX_COMPILER_OPTION([-Wno-multichar],
                               [CXXFLAGS="$CXXFLAGS -Wno-multichar"])
  fi

  case $host in
  *-*-irix*) 
    ### Turn on all warnings ######################################
    if test x"$CC" = xcc || test x"$CC" = xCC; then
      SIM_AC_CC_COMPILER_OPTION([-fullwarn], [CFLAGS="$CFLAGS -fullwarn"])
    fi
    if test x"$CXX" = xCC; then
      SIM_AC_CXX_COMPILER_OPTION([-fullwarn], [CXXFLAGS="$CXXFLAGS -fullwarn"])
    fi

    ### Turn off specific (bogus) warnings ########################

    ### SGI MipsPro v?.?? (our compiler on IRIX 6.2) ##############
    ##
    ## 3115: ``type qualifiers are meaningless in this declaration''.
    ## 3262: unused variables.
    ##
    ### SGI MipsPro v7.30 #########################################
    ##
    ## 1174: "The function was declared but never referenced."
    ## 1209: "The controlling expression is constant." (kill warning on
    ##       if (0), assert(FALSE), etc).
    ## 1355: Kill warnings on extra semicolons (which happens with some
    ##       of the Coin macros).
    ## 1375: Non-virtual destructors in base classes.
    ## 3201: Unused argument to a function.
    ## 1110: "Statement is not reachable" (the Lex/Flex generated code in
    ##       Coin/src/engines has lots of shitty code which needs this).
    ## 1506: Implicit conversion from "unsigned long" to "long".
    ##       SbTime.h in SGI/TGS Inventor does this, so we need to kill
    ##       this warning to avoid all the output clutter when compiling
    ##       the SoQt, SoGtk or SoXt libraries on IRIX with SGI MIPSPro CC.

    sim_ac_bogus_warnings="-woff 3115,3262,1174,1209,1355,1375,3201,1110,1506"

    if test x"$CC" = xcc || test x"$CC" = xCC; then
      SIM_AC_CC_COMPILER_OPTION([$sim_ac_bogus_warnings],
                                [CFLAGS="$CFLAGS $sim_ac_bogus_warnings"])
    fi
    if test x"$CXX" = xCC; then
      SIM_AC_CXX_COMPILER_OPTION([$sim_ac_bogus_warnings],
                                 [CXXFLAGS="$CXXFLAGS $sim_ac_bogus_warnings"])
    fi
  ;;
  esac
fi
])
