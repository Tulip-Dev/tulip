SET(PLUGIN_NAME ReverseEdges)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipGUIInclude})

SET(PLUGIN_SRCS
 ReverseEdges.cpp
)

ADD_LIBRARY(${PLUGIN_NAME}-${Tulip_VERSION} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${Tulip_VERSION} ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(${PLUGIN_NAME}-${Tulip_VERSION} ${TulipPluginsInstallDir})
