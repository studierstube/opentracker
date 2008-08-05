#FILE(GLOB_RECURSE HEADERS common/*.h core/*.h input/*.h misc/*.h network/*.h prop/*.h prop/rigidTransform/*.h prop/tnt/*.h tool/*.h)
#SET(OT_HEADERS ${HEADERS} CACHE INTERNAL "")

IF(OT_USE_OTQT)
FILE(GLOB_RECURSE QT_HEADERS otqt/*.h)
SET(OT_QT_HEADERS ${QT_HEADERS} CACHE INTERNAL "")
ENDIF(OT_USE_OTQT)

INSTALL(DIRECTORY . DESTINATION include/OpenTracker FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
