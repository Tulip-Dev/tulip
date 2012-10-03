ADD_EXECUTABLE(graph_display main.cpp)
TARGET_LINK_LIBRARIES(graph_display ${TulipLibs})

INSTALL(TARGETS graph_display DESTINATION demos)
