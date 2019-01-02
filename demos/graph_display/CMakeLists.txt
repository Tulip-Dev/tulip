INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

ADD_EXECUTABLE(graph_display graph_display.cpp)
TARGET_LINK_LIBRARIES(graph_display ${TulipLibs} ${QT_LIBRARIES})

INSTALL(TARGETS graph_display DESTINATION demos)
