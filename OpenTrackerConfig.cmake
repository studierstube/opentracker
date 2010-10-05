# - Configure the opentracker library
# Once done this will define
#
#  OPENTRACKER_FOUND - system has OPENTRACKER
#  OPENTRACKER_DEFINITIONS - definitions for OPENTRACKER
#  OPENTRACKER_INCLUDE_DIRS - the OPENTRACKER include directories
#  OPENTRACKER_LIBRARIES - Link these to use OPENTRACKER
#  OPENTRACKER_LIBRARY_DIRS - link directories, useful for rpath
#

if(OPENTRACKER_INCLUDE_DIRS AND OPENTRACKER_LIBRARIES)

  # in cache already:
  set(OPENTRACKER_FOUND TRUE)

else(OPENTRACKER_INCLUDE_DIRS AND OPENTRACKER_LIBRARIES)

  get_filename_component(OPENTRACKER_ROOT "${CMAKE_CURRENT_LIST_FILE}" PATH)

  if(NOT EXISTS ${OPENTRACKER_ROOT}/CMakeLists.txt)
    # we are *not* in the source tree, so find the actual root from here:
    get_filename_component(OPENTRACKER_ROOT "${OPENTRACKER_ROOT}" PATH)
    get_filename_component(OPENTRACKER_ROOT "$ENV{OpenTracker_DIR}" PATH)
  endif(NOT EXISTS ${OPENTRACKER_ROOT}/CMakeLists.txt)

  set(OPENTRACKER_INCLUDE_DIRS "${OPENTRACKER_ROOT}/include" CACHE STRING "OpenTracker include directories")
  #set(OPENTRACKER_LIBRARIES "opentracker" CACHE STRING "OpenTracker library name")
  set(OPENTRACKER_DEFINITIONS "-DUSE_OT_2_0" CACHE STRING "OpenTracker definitions")


 SET(POTENTIAL_LIBRARY_PATHS
    ${OPENTRACKER_ROOT}/src 
       ${OPENTRACKER_ROOT}/lib 
       ${OPENTRACKER_ROOT}/lib/Release 
       ${OPENTRACKER_ROOT}/lib${LIBSUFFIX} 
       ${OPENTRACKER_ROOT}/lib64 
       ${OPENTRACKER_ROOT}/lib/win32 
       ${OPENTRACKER_ROOT}/lib/win32/${CMAKE_CFG_INTDIR} 
       $ENV{OpenTracker_DIR}/lib/win32/${CMAKE_CFG_INTDIR}
       # make sure not to mix locally installed headers with globally installed binaries:
    NO_DEFAULT_PATH
  )

  SET(LIBRARY_OPENTRACKER_RELEASE_NAMES opentracker)

  FIND_LIBRARY(OPENTRACKER_LIBRARIES
    NAMES ${LIBRARY_OPENTRACKER_RELEASE_NAMES}
    PATHS ${POTENTIAL_LIBRARY_PATHS}
    PATH_SUFFIXES release Release
    NO_DEFAULT_PATH
  )
  LIST(APPEND CUDA_RENDERER_LIBRARY_LIST ${LIBRARY_CUDARENDERER_RELEASE} )
  MARK_AS_ADVANCED(LIBRARY_CUDARENDERER_RELEASE)

  if(OPENTRACKER_LIBRARIES)
    # store library directories in cache:
    get_filename_component(OPENTRACKER_LIBRARY_PATH ${OPENTRACKER_LIBRARIES} PATH)
    set(OPENTRACKER_LIBRARY_DIRS "${OPENTRACKER_LIBRARY_PATH}" CACHE STRING "OpenTracker library directories")
    set(OPENTRACKER_FOUND TRUE)
  endif(OPENTRACKER_LIBRARIES)

  if(OPENTRACKER_FOUND)
    if(NOT OpenTracker_FIND_QUIETLY)
      message(STATUS "Found OpenTracker: ${OPENTRACKER_LIBRARY_DIRS} ${OPENTRACKER_INCLUDE_DIRS}")
    endif(NOT OpenTracker_FIND_QUIETLY)
  else(OPENTRACKER_FOUND)
    if(OpenTracker_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find OpenTracker")
    endif(OpenTracker_FIND_REQUIRED)
  endif(OPENTRACKER_FOUND)

endif(OPENTRACKER_INCLUDE_DIRS AND OPENTRACKER_LIBRARIES)
