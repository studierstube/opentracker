
project(opentracker)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true) # Avoid unneccesary code
cmake_minimum_required(VERSION 2.6)

file(TO_CMAKE_PATH "$ENV{ICG_CMAKE_MODULE_PATH}" OT_CMAKE_MODULE_PATH)
set(CMAKE_MODULE_PATH ${OT_CMAKE_MODULE_PATH})

# specify the lib and dll output folders for win32
if(WIN32)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${opentracker_SOURCE_DIR}/bin/win32)
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${opentracker_SOURCE_DIR}/lib/win32)
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${opentracker_SOURCE_DIR}/lib/win32)
endif(WIN32)

if(UNIX)
   include(UsePkgConfig)
endif(UNIX)

find_package(ACE REQUIRED)
find_package(ARToolKitPlus REQUIRED)
find_package(OpenGL REQUIRED)

find_package(Xercesc)
if (NOT XERCESC_FOUND)
  find_package(TinyXMLMod REQUIRED)
  option(OT_USE_TINYXML "Build OT with TinyXML as standard library." ON)
  option(OT_USE_TINYXML_MOD "Build OT to replace TinyXML by TinyXML_Mod." ON)
  #if(WIN32)
  #  option(OT_TINYXML_DLL "compile and link against the DLL version of TinyXMLMod." ON)
  #else(WIN32)
  #  option(OT_TINYXML_DLL "compile and link against the DLL version of TinyXMLMod." OFF)
  #endif(WIN32)
else (NOT XERCESC_FOUND)
  option(OT_USE_TINYXML "Build OT with TinyXML as standard library." OFF)
  option(OT_USE_TINYXML_MOD "Build OT to replace TinyXML by TinyXML_Mod." OFF)
endif (NOT XERCESC_FOUND)
  
if(UNIX)
  find_package(Curses)
  add_definitions(-DLINUX)
endif(UNIX)

set(CPACK_GENERATOR "TBZ2")
set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(CPACK_PACKAGE_NAME "opentracker")

# RPM variables:
if(NOT CMAKE_INSTALL_BINDIR)
set(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
endif(NOT CMAKE_INSTALL_BINDIR)
if(NOT CMAKE_INSTALL_LIBDIR)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
endif(NOT CMAKE_INSTALL_LIBDIR)
if(NOT CMAKE_INSTALL_DATADIR)
set(CMAKE_INSTALL_DATADIR ${CMAKE_INSTALL_PREFIX}/share)
endif(NOT CMAKE_INSTALL_DATADIR)
if(NOT CMAKE_INSTALL_DOCDIR)
set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATADIR}/doc/packages)
endif(NOT CMAKE_INSTALL_DOCDIR)
if(NOT CMAKE_INSTALL_MANDIR)
set(CMAKE_INSTALL_MANDIR ${CMAKE_INSTALL_DATADIR}/man)
endif(NOT CMAKE_INSTALL_MANDIR)

if(UNIX)
   # configure opentracker.pc:
   set(OPENTRACKER_PREFIX ${CMAKE_INSTALL_PREFIX})
   set(OPENTRACKER_LIBDIR ${CMAKE_INSTALL_LIBDIR})
   set(OPENTRACKER_PROJECT_NAME ${CPACK_PACKAGE_NAME})
   set(OPENTRACKER_PROJECT_VERSION ${CPACK_PACKAGE_VERSION})
   set(OPENTRACKER_PROJECT_DESCRIPTION "An open architecture for reconfigurable tracking based on XML")
   set(OPENTRACKER_PROJECT_LIBNAME ${CPACK_PACKAGE_NAME})
   set(OPENTRACKER_PROJECT_DEFINES "")
   
   configure_file(opentracker.pc.in opentracker.pc @ONLY)
   install(FILES ${CMAKE_BINARY_DIR}/opentracker.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
   configure_file(redhat/opentracker.spec.in redhat/opentracker.spec)
endif(UNIX)

option(OT_PRESENCCIA_BUILD "Build OT with CORBA Support for PRESENCCIA" OFF)

option(OT_PROPREGISTRATIONMOUDLE_SUPPORT "Build OT with prop registration support. Works only under windows." OFF)
option(OT_USE_ARTOOLKIT "Build OT with ARTOOLKIT support." OFF)
if(ARToolkitPlus_FOUND)
   option(OT_USE_ARTOOLKITPLUS "Build OT with ARToolkitPlus support." ON)
else(ARToolkitPlus_FOUND)
   option(OT_USE_ARTOOLKITPLUS "Build OT with ARToolkitPlus support." OFF)
endif(ARToolkitPlus_FOUND)
#if(WIN32)
#   #option(OT_ARTOOLKITPLUS_DLL "compile and link against the DLL version of ARToolKitPlus." ON)
#else(WIN32)
#   #option(OT_ARTOOLKITPLUS_DLL "compile and link against the DLL version of ARToolKitPlus." OFF)
#endif(WIN32)
option(OT_USE_OPENVIDEO "Build OT with an integrated OpenVideo server." OFF)
#option(OT_USE_TINYXML "Build OT with TinyXML as standard library." OFF)
option(OT_EXCLUDE_MOBILAB_SUPPORT " Set ON in order not to compile support for the driver g.Tec mobilab device  (it requires ACE >= 5.5.6, Windows people love their old 5.5.1, that's why it is disabled by default)" ON)
option(OT_USE_MSDEV_DEBUGOUTPUT "Build OT to direct console output to the MSDev debug console." OFF)
option(OT_USE_CYBERMOUSE "Build OT to compile support for CyberMouse device." OFF)
option(OT_USE_WACOMGRAPHIRE "Build OT to compile support for Wacom Graphire tablet. Support for this is only available on windows." OFF)
option(OT_USE_JOYSTICK "Build OT  to compile support for Joysticks." OFF)
option(OT_USE_SPACEDEVICE "Build OT to compile support for 3Dconnexion." OFF)
option(OT_USE_GOGO  "Build OT to compile support for GoGo interaction support." OFF)
option(OT_USE_PANTILTUNIT  "Build OT to compile support for Pan Tilt Units. Support for this is only available on windows at the moment." OFF)
option(OT_USE_SAPISPEECH  "Build OT to compile support for SAPI Speech Recognition and Text to Speech. Support for this is only available on windows. Otherwise an empty implementation will be used " OFF)
option(OT_USE_P5GLOVE  "Build OTto compile support for Essentialreality's P5 glove. Support for this is only available on windows yet. Linux support is in development." OFF)
option(OT_USE_DWARF  "Build OT to compile support for the DWARF." OFF)
option(OT_USE_VRPN  "Build OT to compile support for VRPN. Don't forget to set the VRPNROOT environment variable !." OFF)
option(OT_USE_UBISENSE  "Build OTto compile support for the Ubisense system. Support for this is only available on windows." OFF)
option(OT_USE_RAWINPUT  "Build OT to compile support for raw input. Support for this is only available on windows." OFF)
option(OT_USE_MIDI  "Build OT to compile support for the MIDI library." OFF)
option(OT_OLD_NETWORK_SUPPORT   "Build OT to compile support for old (1.1) Opentracker node." OFF)
option(OT_USE_PYTHON  "Build OT to compile support for Python." OFF)
option(OT_USE_OTQT  "Build OT  to compile support for the OTQt." OFF)
option(OT_USE_LASERPOINTER  "Build OT  to compile support for the laser pointer." OFF)


if(OT_PRESENCCIA_BUILD)
  set(OT_USE_OTQT "ON")
endif(OT_PRESENCCIA_BUILD)

# the following line breaks the source package generation with CPack:
#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin )

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin )
#set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin )

#message("PROJECT_NAME=${PROJECT_NAME}")
#message("test_SOURCE_DIR=${test_SOURCE_DIR}")
#message("EXECUTABLE_OUTPUT_PATH=${EXECUTABLE_OUTPUT_PATH}")
#message("PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
#message("PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")
#message("CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
#message("CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")


if(OT_USE_PYTHON)
  add_definitions(-DUSE_PYTHON)
  find_package(SWIG REQUIRED)
  include(${SWIG_USE_FILE})
  find_package(PythonLibs REQUIRED)
  include_directories(${PYTHON_INCLUDE_PATH})
  set(CMAKE_SWIG_FLAGS "")
endif(OT_USE_PYTHON)



if(WIN32)
   add_subdirectory(extras)
endif(WIN32)
add_subdirectory(include)
add_subdirectory(share)



set(BUILD_SHARED_LIBS ON CACHE DOC "build the shared lib instead of the static?")

if(WIN32)
   add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
   add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
   add_definitions(-D_CRT_SECURE_NO_DEPRECATE_GLOBALS)
   # configure debug and static library postfixes
   if(BUILD_SHARED_LIBS)
      set(CMAKE_DEBUG_POSTFIX "d")
   else(BUILD_SHARED_LIBS)
      set(CMAKE_DEBUG_POSTFIX "d_static")
      set(CMAKE_RELEASE_POSTFIX "_static")
      # enable the DLL code of opentracker
      add_definitions(-DOPENTRACKER_STATIC)
   endif(BUILD_SHARED_LIBS)
endif(WIN32)

# XML
if(XERCESC_FOUND)
   include_directories(${XERCESC_INCLUDE_DIR})
   link_directories(${XERCESC_LIBRARY_DIR})
   add_definitions(-DUSE_XERCES)
else(XERCESC_FOUND)
   include_directories(${TINY_XML_MOD_INCLUDE_DIR})
   link_directories(${TINY_XML_MOD_LIBRARY_DIR})
   add_definitions(${TINY_XML_MOD_DEFINITIONS})
endif(XERCESC_FOUND)

# ARToolkitPlus
if(OT_USE_ARTOOLKITPLUS)
   include_directories(${ARToolkitPlus_INCLUDE_DIR})
   link_directories(${ARToolkitPlus_LIBRARY_DIR})
   add_definitions(${ARToolkitPlus_DEFINITIONS})
endif(OT_USE_ARTOOLKITPLUS)

#### ACE
include_directories(${ACE_INCLUDE_DIR})
link_directories(${ACE_LIBRARY_DIR})
## old versions of ace (contained in Ubuntu 7.04 and 8.04) 
## require additional CPP flag for compilation
if(UNIX)
   if(ACE_VERSION STREQUAL "5.4.7")
       add_definitions(-D_REENTRANT)
   endif(ACE_VERSION STREQUAL "5.4.7")
endif(UNIX)

# OpenTracker itself
include_directories(include extras)

add_subdirectory(src)

install(FILES OpenTrackerConfig.cmake DESTINATION lib/OpenTracker)

## packaging:
if(WIN32 AND NOT CYGWIN)
  set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\opentracker.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} opentracker")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.opentracker.org")
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_MODULE_PATH ${CMAKE_MODULE_PATH})
else(WIN32 AND NOT CYGWIN)
  set(CPACK_STRIP_FILES "bin/opentracker")
  set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT CYGWIN)


set(CPACK_IGNORE_FILES
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

set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
include(CPack)

add_custom_target(dist COMMAND make clean && make package_source)
add_custom_target(clean-all COMMAND find \\\( -name CMakeFiles -or -name CMakeCache.txt -or -name Makefile -or -name '*.cmake' -or -name _CPack_Packages -or -name '*~' \\\) -prune -exec rm -rf {} '\;')
