link_libraries(opentracker)

add_executable(middleware middleware.cxx)
target_link_libraries(middleware ${ARToolkitPlus_LIBRARY}) 
add_executable(opentracker-bin main.cxx)
#add_executable(opentracker2c) ???
add_executable(opentrackerdemand main_ondemand.cxx)
target_link_libraries(opentrackerdemand ${ARToolkitPlus_LIBRARY}) 
#add_executable(opentrackerubitrack main_ubitrack.cxx) ???

install(TARGETS middleware opentracker-bin opentrackerdemand DESTINATION ${CMAKE_INSTALL_BINDIR})
