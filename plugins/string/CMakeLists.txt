INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})
ADD_LIBRARY(ToLabels-${TulipVersion} SHARED ToLabels.cpp)
TARGET_LINK_LIBRARIES(ToLabels-${TulipVersion} ${LibTulipCoreName})
INSTALL_TULIP_PLUGIN(ToLabels-${TulipVersion} ${TulipPluginsInstallDir})
