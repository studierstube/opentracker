PROJECT(opentracker)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

FILE(TO_CMAKE_PATH "$ENV{CMAKE_MODULE_PATH}" OT_CMAKE_MODULE_PATH)

SET(CMAKE_MODULE_PATH ${OT_CMAKE_MODULE_PATH})

INCLUDE(UsePkgConfig)

SET(CMAKE_BUILD_TYPE "RelWithDebInfo")

#FIND_PACKAGE(OpenGL REQUIRED)
#FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(ACE REQUIRED)
FIND_PACKAGE(TinyXMLMod REQUIRED)
FIND_PACKAGE(Xercesc REQUIRED)
FIND_PACKAGE(ARToolKitPlus REQUIRED)
#FIND_PACKAGE(AVIFile64 REQUIRED)

IF(UNIX)
  FIND_PACKAGE(Curses)
  ADD_DEFINITIONS(-DLINUX)
ENDIF(UNIX)

SET(CPACK_GENERATOR "TBZ2")
SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_PACKAGE_VERSION_MAJOR "2")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
SET(CPACK_PACKAGE_NAME "opentracker")

# RPM variables:
IF(NOT CMAKE_INSTALL_BINDIR)
SET(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF(NOT CMAKE_INSTALL_BINDIR)
IF(NOT CMAKE_INSTALL_LIBDIR)
SET(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF(NOT CMAKE_INSTALL_LIBDIR)
IF(NOT CMAKE_INSTALL_DATADIR)
SET(CMAKE_INSTALL_DATADIR ${CMAKE_INSTALL_PREFIX}/share)
ENDIF(NOT CMAKE_INSTALL_DATADIR)
IF(NOT CMAKE_INSTALL_DOCDIR)
SET(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATADIR}/doc/packages)
ENDIF(NOT CMAKE_INSTALL_DOCDIR)
IF(NOT CMAKE_INSTALL_MANDIR)
SET(CMAKE_INSTALL_MANDIR ${CMAKE_INSTALL_DATADIR}/man)
ENDIF(NOT CMAKE_INSTALL_MANDIR)

# configure opentracker.pc:
SET(OPENTRACKER_PREFIX ${CMAKE_INSTALL_PREFIX})
SET(OPENTRACKER_LIBDIR ${CMAKE_INSTALL_LIBDIR})
SET(OPENTRACKER_PROJECT_NAME ${CPACK_PACKAGE_NAME})
SET(OPENTRACKER_PROJECT_VERSION ${CPACK_PACKAGE_VERSION})
SET(OPENTRACKER_PROJECT_DESCRIPTION "An open architecture for reconfigurable tracking based on XML")
SET(OPENTRACKER_PROJECT_LIBNAME ${CPACK_PACKAGE_NAME})
SET(OPENTRACKER_PROJECT_DEFINES "")
CONFIGURE_FILE(opentracker.pc.in opentracker.pc @ONLY)
INSTALL(FILES opentracker.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

CONFIGURE_FILE(redhat/opentracker.spec.in redhat/opentracker.spec)

INCLUDE_DIRECTORIES(include ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR} ${ACE_INCLUDE_DIR} ${TINY_XML_MOD_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/extras ${XERCESC_INCLUDE_DIR} ${ARToolkitPlus_INCLUDE_DIR})

OPTION(OT_PRESENCCIA_BUILD "Build OT with CORBA Support for PRESENCCIA" OFF)

OPTION(OT_PROPREGISTRATIONMOUDLE_SUPPORT "Build OT with prop registration support. Works only under windows." OFF)
OPTION(OT_USE_ARTOOLKIT "Build OT with ARTOOLKIT support." OFF)
IF(WIN32)
OPTION(OT_ARTOOLKITPLUS_DLL "compile and link against the DLL version of ARToolKitPlus." ON)
OPTION(OT_USE_ARTOOLKITPLUS "Build OT with ARTOOLKIT support." ON)
ELSE(WIN32)
OPTION(OT_ARTOOLKITPLUS_DLL "compile and link against the DLL version of ARToolKitPlus." OFF)
OPTION(OT_USE_ARTOOLKITPLUS "Build OT with ARTOOLKIT support." OFF)
ENDIF(WIN32)
OPTION(OT_USE_OPENVIDEO "Build OT with an integrated OpenVideo server." OFF)
OPTION(OT_USE_TINYXML "Build OT with TinyXML as standard library." OFF)
OPTION(OT_EXCLUDE_MOBILAB_SUPPORT " Set ON in order not to compile support for the driver g.Tec mobilab device  (it requires ACE >= 5.5.6, Windows people love their old 5.5.1, that's why it is disabled by default)" OFF)
OPTION(OT_USE_MSDEV_DEBUGOUTPUT "Build OT to direct console output to the MSDev debug console." OFF)
OPTION(OT_USE_CYBERMOUSE "Build OT to compile support for CyberMouse device." OFF)
OPTION(OT_USE_WACOMGRAPHIRE "Build OT to compile support for Wacom Graphire tablet. Support for this is only available on windows." OFF)
OPTION(OT_USE_JOYSTICK "Build OT  to compile support for Joysticks." OFF)
OPTION(OT_USE_SPACEDEVICE "Build OT to compile support for 3Dconnexion." OFF)
OPTION(OT_USE_GOGO  "Build OT to compile support for GoGo interaction support." OFF)
OPTION(OT_USE_PANTILTUNIT  "Build OT to compile support for Pan Tilt Units. Support for this is only available on windows at the moment." OFF)
OPTION(OT_USE_SAPISPEECH  "Build OT to compile support for SAPI Speech Recognition and Text to Speech. Support for this is only available on windows. Otherwise an empty implementation will be used " OFF)
OPTION(OT_USE_P5GLOVE  "Build OTto compile support for Essentialreality's P5 glove. Support for this is only available on windows yet. Linux support is in development." OFF)
OPTION(OT_USE_DWARF  "Build OT to compile support for the DWARF." OFF)
OPTION(OT_USE_VRPN  "Build OT to compile support for VRPN. Don't forget to set the VRPNROOT environment variable !." OFF)
OPTION(OT_USE_UBISENSE  "Build OTto compile support for the Ubisense system. Support for this is only available on windows." OFF)
OPTION(OT_USE_RAWINPUT  "Build OT to compile support for raw input. Support for this is only available on windows." OFF)
OPTION(OT_USE_MIDI  "Build OT to compile support for the MIDI library." OFF)
OPTION(OT_OLD_NETWORK_SUPPORT   "Build OT to compile support for old (1.1) Opentracker node." OFF)
OPTION(OT_USE_PYTHON  "Build OT to compile support for Python." OFF)
OPTION(OT_USE_OTQT  "Build OT  to compile support for the OTQt." OFF)


IF(OT_PRESENCCIA_BUILD)
  SET(OT_USE_OTQT "ON")
ENDIF(OT_PRESENCCIA_BUILD)

SET(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin )
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin )
SET(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin )

#MESSAGE("PROJECT_NAME=${PROJECT_NAME}")
#MESSAGE("test_SOURCE_DIR=${test_SOURCE_DIR}")
#MESSAGE("EXECUTABLE_OUTPUT_PATH=${EXECUTABLE_OUTPUT_PATH}")
#MESSAGE("PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
#MESSAGE("PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")
#MESSAGE("CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
#MESSAGE("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")


IF(OT_USE_PYTHON)
  ADD_DEFINITIONS(-DUSE_PYTHON)
  FIND_PACKAGE(SWIG REQUIRED)
  INCLUDE(${SWIG_USE_FILE})
  FIND_PACKAGE(PythonLibs REQUIRED)
  INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
  SET(CMAKE_SWIG_FLAGS "")
ENDIF(OT_USE_PYTHON)


IF(WIN32)
ADD_SUBDIRECTORY(extras)
ENDIF(WIN32)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(share)

INSTALL(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN ".svn" EXCLUDE)

IF(UNIX)
  INSTALL(FILES FindOpenTracker.cmake DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/Modules)
ENDIF(UNIX)

IF(WIN32)
  INSTALL(FILES FindOpenTracker.cmake DESTINATION $ENV{CMAKE_MODULE_PATH})
ENDIF(WIN32)

LINK_DIRECTORIES(${ACE_LIBRARY_DIR} ${TINY_XML_MOD_LIBRARY_DIR} ${ARToolkitPlus_LIBRARY_DIR} ${XERCEXC_LIBRARY_DIR})

# packaging:

SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "2")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

IF(WIN32 AND NOT UNIX)
  SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\opentracker.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} opentracker")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.opentracker.org")
  SET(CPACK_NSIS_MODIFY_PATH ON)
  SET(CPACK_MODULE_PATH ${CMAKE_MODULE_PATH})
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_STRIP_FILES "bin/opentracker")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)


SET(CPACK_IGNORE_FILES
  "/#.*#$"
  "/log"
  "/trunk/build/"
  "CMakeCache\\\\.txt"
  "CMakeFiles"
  "CPack.*Config\\\\.cmake"
  "CVS"
  "Makefile"
  "SCons*"
  "\\\\.#"
  "\\\\.a$"
  "\\\\.aux$"
  "\\\\.bz2$"
  "\\\\.depend"
  "\\\\.deps*"
  "\\\\.dll$"
  "\\\\.dsp$"
  "\\\\.dvi$"
  "\\\\.eps$"
  "\\\\.gz$"
  "\\\\.iv$"
  "\\\\.la$"
  "\\\\.lib$"
  "\\\\.libs$"
  "\\\\.lo$"
  "\\\\.loT$"
  "\\\\.log$"
  "\\\\.o$"
  "\\\\.out"
  "\\\\.pgm$"
  "\\\\.pro$"
  "\\\\.ps$"
  "\\\\.save"
  "\\\\.scon.*"
  "\\\\.so"
  "\\\\.svn"
  "\\\\.swp$"
  "\\\\.tbz$"
  "\\\\.tex$"
  "\\\\.tgz$"
  "\\\\.vcproj$"
  "^/box.cpp"
  "_CPack_Packages"
  "a.out"
  "autom4te.cache"
  "bin"
  "cfg*"
  "cgparams$"
  "cmake_install.cmake"
  "cvs_import"
  "install.sh"
  "install_manifest.txt"
  "makedist.sh"
  "moc_"
  "pkgconfig"
  "progress.make"
  "~$"
)

SET(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
INCLUDE(CPack)

ADD_CUSTOM_TARGET(dist COMMAND make clean && make package_source)
ADD_CUSTOM_TARGET(clean-all COMMAND find \\\( -name CMakeFiles -or -name CMakeCache.txt -or -name Makefile -or -name '*.cmake' -or -name _CPack_Packages -or -name '*~' \\\) -prune -exec rm -rf {} '\;')
