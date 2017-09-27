INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

ADD_LIBRARY(Sunburst-${TulipVersion} SHARED Sunburst.cpp)
TARGET_LINK_LIBRARIES(Sunburst-${TulipVersion} ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(Sunburst-${TulipVersion} ${TulipPluginsInstallDir})
