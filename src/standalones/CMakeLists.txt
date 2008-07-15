LINK_LIBRARIES(opentracker)

ADD_EXECUTABLE(middleware middleware.cxx)
TARGET_LINK_LIBRARIES(middleware ${ARToolkitPlus_LIBRARY}) 
ADD_EXECUTABLE(opentracker-bin main.cxx)
#ADD_EXECUTABLE(opentracker2c) ???
ADD_EXECUTABLE(opentrackerdemand main_ondemand.cxx)
TARGET_LINK_LIBRARIES(opentrackerdemand ${ARToolkitPlus_LIBRARY}) 
#ADD_EXECUTABLE(opentrackerubitrack main_ubitrack.cxx) ???

INSTALL(TARGETS middleware opentracker-bin opentrackerdemand DESTINATION ${CMAKE_INSTALL_BINDIR})
