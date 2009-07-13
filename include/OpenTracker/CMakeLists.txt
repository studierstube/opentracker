#file(GLOB_RECURSE HEADERS common/*.h core/*.h input/*.h misc/*.h network/*.h prop/*.h prop/rigidTransform/*.h prop/tnt/*.h tool/*.h)
#set(OT_HEADERS ${HEADERS} CACHE INTERNAL "")

if(OT_USE_OTQT)
file(GLOB_RECURSE QT_HEADERS otqt/*.h)
set(OT_QT_HEADERS ${QT_HEADERS} CACHE INTERNAL "")
endif(OT_USE_OTQT)

install(DIRECTORY . DESTINATION include/OpenTracker FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
