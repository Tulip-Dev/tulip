CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(PLUGIN_NAME reverseedges)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipGUIInclude})

SET(PLUGIN_SRCS
 reverseedges.cpp
)

ADD_LIBRARY(${PLUGIN_NAME}-${TulipVersion} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TulipVersion} ${LibTulipCoreName})

INSTALL(TARGETS ${PLUGIN_NAME}-${TulipVersion} DESTINATION ${TulipPluginsInstallDir})

