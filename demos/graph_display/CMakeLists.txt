INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${QT_INCLUDE_DIR})

ADD_EXECUTABLE(graph_display graph_display.cpp)
TARGET_LINK_LIBRARIES(graph_display ${TulipLibs} ${QT_LIBRARIES})

INSTALL(TARGETS graph_display DESTINATION demos)
