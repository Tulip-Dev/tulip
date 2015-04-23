SET(PLUGIN_NAME CurveEdges)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

SET(PLUGIN_SRCS
 CurveEdges.cpp
)

ADD_LIBRARY(${PLUGIN_NAME}-${TulipVersion} SHARED ${PLUGIN_SRCS})
TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TulipVersion} ${LibTulipCoreName})

INSTALL_TULIP_PLUGIN(${PLUGIN_NAME}-${TulipVersion} ${TulipPluginsInstallDir})

