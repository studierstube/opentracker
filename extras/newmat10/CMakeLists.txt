 project(newmat)
 file(GLOB_RECURSE SOURCES *.cpp)
 
 add_definitions(-DWIN32 -D_LIB -DNDEBUG)
 add_library(newmat STATIC ${SOURCES})