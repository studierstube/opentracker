file(GLOB_RECURSE OPENTRACKER_SOURCES
  common/*.cxx
  core/*.cxx
  input/*.cxx
  misc/*.cxx
  network/*.cxx
  otpy/*.cxx
  #otqt/*.cxx
  tool/*.cxx
  ubitrack/*.cxx
  prop/rigidTransform/*.cxx
  ../include/OpenTracker/svd/*.cpp
)

file(GLOB_RECURSE OPENTRACKER_HEADERS
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  ../include/OpenTracker/common/*.h
  ../include/OpenTracker/core/*.h
  ../include/OpenTracker/input/*.h
  ../include/OpenTracker/misc/*.h
  ../include/OpenTracker/network/*.h
  ../include/OpenTracker/otpy/*.h
  #../include/OpenTracker/otqt/*.h
  ../include/OpenTracker/tool/*.h
  ../include/OpenTracker/ubitrack/*.h
  ../include/OpenTracker/prop/rigidTransform/*.h
  ../include/OpenTracker/*.h
  ../include/*.h
  ../extras/xsens/*.h
)

if(WIN32)
  file(GLOB_RECURSE OPENTRACKER_SOURCES
    ${OPENTRACKER_SOURCES}
    prop/*.cxx
  )
  file(GLOB_RECURSE OPENTRACKER_HEADERS
    ${OPENTRACKER_HEADERS}
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	../include/OpenTracker/prop/*.h
  )
  add_definitions(-DCMAKE_BUILD)
endif(WIN32)

if(OT_USE_ARTOOLKIT)
  add_definitions(-DUSE_ARTOOLKIT)
endif(OT_USE_ARTOOLKIT)

if(OT_USE_ARTOOLKITPLUS)
  add_definitions(-DUSE_ARTOOLKITPLUS)
  if(WIN32)
     add_definitions(-DOT_DO_NOT_LINK_ARTOOLKITPLUS) # This is a workaround for win32
  endif(WIN32)
endif(OT_USE_ARTOOLKITPLUS)

if(OT_USE_OPENVIDEO)
  add_definitions(-DUSE_OPENVIDEO)
endif(OT_USE_OPENVIDEO)

if(OT_USE_TINYXML)
  add_definitions(-DUSE_TINYXML)
  if(OT_USE_TINYXML_MOD)
     add_definitions(-DTINYXML_MOD)
  endif(OT_USE_TINYXML_MOD)
  if(WIN32)
     add_definitions(-DOT_DO_NOT_LINK_TINYXML_VIA_PRAGMA_COMMENT_LIB) # This is a workaround for win32
  endif(WIN32)
endif(OT_USE_TINYXML)

if(OT_EXCLUDE_MOBILAB_SUPPORT)
  add_definitions(-DEXCLUDE_MOBILAB_SUPPORT)
endif(OT_EXCLUDE_MOBILAB_SUPPORT)

if(OT_USE_MSDEV_DEBUGOUTPUT)
  add_definitions(-DUSE_MSDEV_DEBUGOUTPUT)
endif(OT_USE_MSDEV_DEBUGOUTPUT)

if(OT_USE_CYBERMOUSE)
  add_definitions(-DUSE_CYBERMOUSE)
endif(OT_USE_CYBERMOUSE)

if(OT_USE_WACOMGRAPHIRE)
  add_definitions(-DUSE_WACOMGRAPHIRE)
endif(OT_USE_WACOMGRAPHIRE)

if(OT_USE_JOYSTICK)
  add_definitions(-DUSE_JOYSTICK)
endif(OT_USE_JOYSTICK)

if(OT_USE_SPACEDEVICE)
  add_definitions(-DUSE_SPACEDEVICE)
endif(OT_USE_SPACEDEVICE)

if(OT_USE_GOGO)
  add_definitions(-DUSE_GOGO)
endif(OT_USE_GOGO)

if(OT_USE_PANTILTUNIT)
  add_definitions(-DUSE_PANTILTUNIT)
endif(OT_USE_PANTILTUNIT)

if(OT_USE_SAPISPEECH)
  add_definitions(-DUSE_SAPISPEECH)
endif(OT_USE_SAPISPEECH)

if(OT_USE_P5GLOVE)
  add_definitions(-DUSE_P5GLOVE)
endif(OT_USE_P5GLOVE)

if(OT_USE_DWARF)
  add_definitions(-DUSE_DWARF)
endif(OT_USE_DWARF)

if(OT_USE_VRPN)
  add_definitions(-DUSE_VRPN)
endif(OT_USE_VRPN)

if(OT_USE_UBISENSE)
  add_definitions(-DUSE_UBISENSE)
endif(OT_USE_UBISENSE)

if(OT_USE_RAWINPUT)
  add_definitions(-DUSE_RAWINPUT)
endif(OT_USE_RAWINPUT)

if(OT_USE_MIDI)
  add_definitions(-DUSE_MIDI)
endif(OT_USE_MIDI)

if(OT_OLD_NETWORK_SUPPORT)
  add_definitions(-DOLD_NETWORK_SUPPORT)
endif(OT_OLD_NETWORK_SUPPORT)


if(OT_PRESENCCIA_BUILD)
  set(OT_USE_OTQT "ON")
  add_definitions(-DUSE_OTQT)  
  add_definitions(-DENABLE_CORBA)
endif(OT_PRESENCCIA_BUILD)

if(OT_USE_OTQT)
  add_definitions(-DUSE_OTQT -DUSE_THREETOTWODIMFILTER)
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
    
FILE(GLOB_RECURSE OTQT_Sources otqt/*.cxx)
FILE(GLOB_RECURSE OTQT_header ../include/OpenTracker/otqt/*.h)
  
  qt4_wrap_cpp(OT_MOC_SRCS ${OT_QT_HEADERS} ${OTQT_header})
  message("preparing for moc ${OT_QT_HEADERS} to ${OT_MOC_SRCS}")
endif(OT_USE_OTQT)

if(OT_USE_LASERPOINTER)
  add_definitions(-DUSE_LASERPOINTER)
endif(OT_USE_LASERPOINTER)


if(NOT OT_PROPREGISTRATIONMOUDLE_SUPPORT)
  add_definitions(-DOT_NO_PROPREGISTRATIONMOUDLE_SUPPORT -DOT_NO_STYLUSMODULE_SUPPORT)
endif(NOT OT_PROPREGISTRATIONMOUDLE_SUPPORT)

if(OT_USE_PYTHON)
   #FIXXME: This is not ready yet
   find_package(SWIG REQUIRED)
   include(${SWIG_USE_FILE})

   find_package(PythonLibs)
   include_directories(${PYTHON_INCLUDE_PATH})

   include_directories(${CMAKE_CURRENT_SOURCE_DIR})
   include_directories(../include)

   set(CMAKE_SWIG_FLAGS "")

   set_source_files_properties(otpy/ot.swg PROPERTIES CPLUSPLUS ON)
   set_source_files_properties(otpy/ot.swg PROPERTIES SWIG_FLAGS "-includeall")
   #swig_add_module(ot python otpy/ot.swg)
   #set_target_properties(_ot PROPERTIES LINKER_LANGUAGE CPLUSPLUS)
   #set(CMAKE_CPLUSPLUS_CREATE_SHARED_MODULE TRUE)
   #swig_link_libraries(ot ${PYTHON_LIBRARIES})
endif(OT_USE_PYTHON)

# a few files are making troubles and are excluded for the moment:
list(REMOVE_ITEM OPENTRACKER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/network/CORBAModule.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/network/CORBASink.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/network/CORBAUtils.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/tool/ToolAppWin.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/tool/ToolIOModule.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/tool/tool.cxx
)

set(OPENTRACKER_SOURCES ${OPENTRACKER_SOURCES} ../extras/intersense/isense.c)

add_subdirectory(standalones)
include_directories(../extras/intersense)

if(WIN32 AND BUILD_SHARED_LIBS)
  add_definitions(-DOPENTRACKER_EXPORTS -DOT_NO_GPS_SUPPORT -DOT_NO_PARBUTTON_SUPPORT)
endif(WIN32 AND BUILD_SHARED_LIBS)

if(OT_USE_OTQT)
  add_library(opentracker ${OT_MOC_SRCS} ${OTQT_Sources} ${OPENTRACKER_HEADERS} ${OPENTRACKER_SOURCES})
  target_link_libraries(opentracker ${QT_LIBRARIES})
else(OT_USE_OTQT)
  add_library(opentracker ${OPENTRACKER_HEADERS} ${OPENTRACKER_SOURCES})
endif(OT_USE_OTQT)

target_link_libraries(opentracker ${ACE_LIBRARIES})

if(WIN32)
  target_link_libraries(opentracker newmat)
else(WIN32)
  target_link_libraries(opentracker ${CURSES_LIBRARIES})
endif(WIN32)

if(OT_USE_ARTOOLKITPLUS)
  target_link_libraries(opentracker ${ARToolkitPlus_LIBRARIES})
endif(OT_USE_ARTOOLKITPLUS)
#if(WIN32)
#  target_link_libraries(opentracker  ${TINY_XML_MOD_LIBRARY} ${CURSES_LIBRARIES} ${ARToolkitPlus_LIBRARY} ${ACE_LIBRARY} newmat)
#else(WIN32)
#  target_link_libraries(opentracker ${ACE_LIBRARIES} ${TINY_XML_MOD_LIBRARY} ${CURSES_LIBRARIES})
#endif(WIN32)

if(XERCESC_FOUND)
  target_link_libraries(opentracker ${XERCESC_LIBRARY})
else(XERCESC_FOUND)
  target_link_libraries(opentracker ${TINY_XML_MOD_LIBRARIES} ${TINY_XML_MOD_LIBRARY_DIR})
endif(XERCESC_FOUND)

if(OT_USE_PYTHON)
  add_definitions(-DUSE_PYTHON)
  target_link_libraries(opentracker ${PYTHON_LIBRARY})
 # set_source_files_properties(ot_wrap.i PROPERTIES CPLUSPLUS ON)
 # set_source_files_properties(example.i PROPERTIES SWIG_FLAGS "-includeall")
 # swig_add_module(example python example.i example.cxx)
#  swig_link_libraries(example ${PYTHON_LIBRARIES})
endif(OT_USE_PYTHON)


install(TARGETS opentracker DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(opentracker PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})