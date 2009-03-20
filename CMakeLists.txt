
PROJECT(opentracker)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true) # Avoid unneccesary code
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FILE(TO_CMAKE_PATH "$ENV{ICG_CMAKE_MODULE_PATH}" OT_CMAKE_MODULE_PATH)
SET(CMAKE_MODULE_PATH ${OT_CMAKE_MODULE_PATH})

# specify the lib and dll output folders for win32
IF(WIN32)
   SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${opentracker_SOURCE_DIR}/bin/win32)
   SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${opentracker_SOURCE_DIR}/lib/win32)
   SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${opentracker_SOURCE_DIR}/lib/win32)
ENDIF(WIN32)

IF(UNIX)
   INCLUDE(UsePkgConfig)
ENDIF(UNIX)

FIND_PACKAGE(ACE REQUIRED)
FIND_PACKAGE(ARToolKitPlus REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

FIND_PACKAGE(Xercesc)
IF (NOT XERCESC_FOUND)
  FIND_PACKAGE(TinyXMLMod REQUIRED)
  OPTION(OT_USE_TINYXML "Build OT with TinyXML as standard library." ON)
  OPTION(OT_USE_TINYXML_MOD "Build OT to replace TinyXML by TinyXML_Mod." ON)
  #IF(WIN32)
  #  OPTION(OT_TINYXML_DLL "compile and link against the DLL version of TinyXMLMod." ON)
  #ELSE(WIN32)
  #  OPTION(OT_TINYXML_DLL "compile and link against the DLL version of TinyXMLMod." OFF)
  #ENDIF(WIN32)
ELSE (NOT XERCESC_FOUND)
  OPTION(OT_USE_TINYXML "Build OT with TinyXML as standard library." OFF)
  OPTION(OT_USE_TINYXML_MOD "Build OT to replace TinyXML by TinyXML_Mod." OFF)
ENDIF (NOT XERCESC_FOUND)
  
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

IF(UNIX)
   # configure opentracker.pc:
   SET(OPENTRACKER_PREFIX ${CMAKE_INSTALL_PREFIX})
   SET(OPENTRACKER_LIBDIR ${CMAKE_INSTALL_LIBDIR})
   SET(OPENTRACKER_PROJECT_NAME ${CPACK_PACKAGE_NAME})
   SET(OPENTRACKER_PROJECT_VERSION ${CPACK_PACKAGE_VERSION})
   SET(OPENTRACKER_PROJECT_DESCRIPTION "An open architecture for reconfigurable tracking based on XML")
   SET(OPENTRACKER_PROJECT_LIBNAME ${CPACK_PACKAGE_NAME})
   SET(OPENTRACKER_PROJECT_DEFINES "")
   
   CONFIGURE_FILE(opentracker.pc.in opentracker.pc @ONLY)
   INSTALL(FILES ${CMAKE_BINARY_DIR}/opentracker.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
   CONFIGURE_FILE(redhat/opentracker.spec.in redhat/opentracker.spec)
ENDIF(UNIX)

OPTION(OT_PRESENCCIA_BUILD "Build OT with CORBA Support for PRESENCCIA" OFF)

OPTION(OT_PROPREGISTRATIONMOUDLE_SUPPORT "Build OT with prop registration support. Works only under windows." OFF)
OPTION(OT_USE_ARTOOLKIT "Build OT with ARTOOLKIT support." OFF)
IF(ARToolkitPlus_FOUND)
   OPTION(OT_USE_ARTOOLKITPLUS "Build OT with ARToolkitPlus support." ON)
ELSE(ARToolkitPlus_FOUND)
   OPTION(OT_USE_ARTOOLKITPLUS "Build OT with ARToolkitPlus support." OFF)
ENDIF(ARToolkitPlus_FOUND)
#IF(WIN32)
#   #OPTION(OT_ARTOOLKITPLUS_DLL "compile and link against the DLL version of ARToolKitPlus." ON)
#ELSE(WIN32)
#   #OPTION(OT_ARTOOLKITPLUS_DLL "compile and link against the DLL version of ARToolKitPlus." OFF)
#ENDIF(WIN32)
OPTION(OT_USE_OPENVIDEO "Build OT with an integrated OpenVideo server." OFF)
#OPTION(OT_USE_TINYXML "Build OT with TinyXML as standard library." OFF)
OPTION(OT_EXCLUDE_MOBILAB_SUPPORT " Set ON in order not to compile support for the driver g.Tec mobilab device  (it requires ACE >= 5.5.6, Windows people love their old 5.5.1, that's why it is disabled by default)" ON)
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
OPTION(OT_USE_LASERPOINTER  "Build OT  to compile support for the laser pointer." OFF)


IF(OT_PRESENCCIA_BUILD)
  SET(OT_USE_OTQT "ON")
ENDIF(OT_PRESENCCIA_BUILD)

# the following line breaks the source package generation with CPack:
#SET(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin )

#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin )
#SET(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin )

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
ADD_SUBDIRECTORY(share)



SET(BUILD_SHARED_LIBS ON CACHE DOC "build the shared lib instead of the static?")

IF(WIN32)
   ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
   ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE)
   ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE_GLOBALS)
   # configure debug and static library postfixes
   IF(BUILD_SHARED_LIBS)
      SET(CMAKE_DEBUG_POSTFIX "d")
   ELSE(BUILD_SHARED_LIBS)
      SET(CMAKE_DEBUG_POSTFIX "d_static")
      SET(CMAKE_RELEASE_POSTFIX "_static")
      # enable the DLL code of opentracker
      ADD_DEFINITIONS(-DOPENTRACKER_STATIC)
   ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)

# XML
IF(XERCESC_FOUND)
   INCLUDE_DIRECTORIES(${XERCESC_INCLUDE_DIR})
   LINK_DIRECTORIES(${XERCESC_LIBRARY_DIR})
   ADD_DEFINITIONS(-DUSE_XERCES)
ELSE(XERCESC_FOUND)
   INCLUDE_DIRECTORIES(${TINY_XML_MOD_INCLUDE_DIR})
   LINK_DIRECTORIES(${TINY_XML_MOD_LIBRARY_DIR})
   ADD_DEFINITIONS(${TINY_XML_MOD_DEFINITIONS})
ENDIF(XERCESC_FOUND)

# ARToolkitPlus
IF(OT_USE_ARTOOLKITPLUS)
   INCLUDE_DIRECTORIES(${ARToolkitPlus_INCLUDE_DIR})
   LINK_DIRECTORIES(${ARToolkitPlus_LIBRARY_DIR})
   ADD_DEFINITIONS(${ARToolkitPlus_DEFINITIONS})
ENDIF(OT_USE_ARTOOLKITPLUS)

#### ACE
INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIR})
LINK_DIRECTORIES(${ACE_LIBRARY_DIR})
## Old versions of Ace (contained in Ubuntu 7.04 and 8.04) 
## require additional CPP flag for compilation
IF(UNIX)
   IF(ACE_VERSION STREQUAL "5.4.7")
       ADD_DEFINITIONS(-D_REENTRANT)
   ENDIF(ACE_VERSION STREQUAL "5.4.7")
ENDIF(UNIX)

# OpenTracker itself
INCLUDE_DIRECTORIES(include extras)

ADD_SUBDIRECTORY(src)

INSTALL(FILES OpenTrackerConfig.cmake DESTINATION lib/OpenTracker)

## packaging:
IF(WIN32 AND NOT CYGWIN)
  SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\opentracker.exe")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} opentracker")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.opentracker.org")
  SET(CPACK_NSIS_MODIFY_PATH ON)
  SET(CPACK_MODULE_PATH ${CMAKE_MODULE_PATH})
ELSE(WIN32 AND NOT CYGWIN)
  SET(CPACK_STRIP_FILES "bin/opentracker")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT CYGWIN)


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
