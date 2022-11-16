SET(PLUGIN_NAME BidirectionalEdges)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

SET(PLUGIN_SRCS
 BidirectionalEdges.cpp
)

ADD_LIBRARY(${PLUGIN_NAME}-${TulipVersion} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TulipVersion} ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(${PLUGIN_NAME}-${TulipVersion} ${TulipPluginsInstallDir})

