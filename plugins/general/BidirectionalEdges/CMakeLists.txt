SET(PLUGIN_NAME BidirectionalEdges)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

SET(PLUGIN_SRCS
 BidirectionalEdges.cpp
)

ADD_LIBRARY(${PLUGIN_NAME}-${Tulip_VERSION} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${Tulip_VERSION} ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(${PLUGIN_NAME}-${Tulip_VERSION} ${TulipPluginsInstallDir})

